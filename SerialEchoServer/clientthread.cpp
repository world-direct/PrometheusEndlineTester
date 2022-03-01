#include "clientthread.h"
#include <QSerialPort>
#include <QSettings>
#include <QTime>

#include "SerialEchoServer_global.h"

ClientThread::ClientThread(QObject *parent)
    :QThread(parent),
   m_portName(),
   m_baudRate(),
   m_dataBits(),
   m_parity(),
   m_stopBits(),
   m_flowControl(),
//   m_response(),
   m_waitTimeout(-1),
   m_mutex(),
   m_quit(false)
{

}

void ClientThread::startClient()
{
    const QMutexLocker locker(&m_mutex);

    QSettings settings(SES_SETT_FILE_NAME, QSettings::IniFormat);
    auto port = settings.value(KEY_SERIAL_PORT).toString();
    auto baudRate = settings.value(KEY_SERIAL_BAUD).toInt();
    auto dataBits = settings.value(KEY_SERIAL_DATA).toInt();
    auto parity = settings.value(KEY_SERIAL_PARITY).toInt();
    auto stopBits = settings.value(KEY_SERIAL_STOP).toInt();
    auto flowControl = settings.value(KEY_SERIAL_FLOWCTRL).toInt();
    auto wait_timeout = settings.value(KEY_SERIAL_WAIT_TIMEOUT).toInt();

    m_portName = port;
    m_baudRate = baudRate;
    m_dataBits = dataBits;
    m_parity = parity;
    m_stopBits = stopBits;
    m_flowControl = flowControl;
    m_waitTimeout = wait_timeout;

//    m_portName = portName;
//    m_waitTimeout = waitTimeout;
//    m_response = response;

    if (!isRunning()){
        start();
    }
}

void ClientThread::run()
{
    bool currentPortNameChanged = false;

    m_mutex.lock();
    QString currentPortName;
    if (currentPortName != m_portName) {
        currentPortName = m_portName;
        currentPortNameChanged = true;
    }

    auto currentBaudRate = m_baudRate;
    auto currentDataBits = m_dataBits;
    auto currentParity = m_parity;
    auto currentStopBits = m_stopBits;
    auto currentFlowControl = m_flowControl;

    int currentWaitTimeout = m_waitTimeout;
//    QString currentRespone = m_response;
    m_mutex.unlock();

    QSerialPort serial;

     while (!m_quit) {
         QThread::currentThread()->sleep(1);
         if (currentPortNameChanged) {
             serial.close();
             serial.setPortName(currentPortName);
             serial.setBaudRate(static_cast<QSerialPort::BaudRate>(currentBaudRate));
             serial.setDataBits(static_cast<QSerialPort::DataBits>(currentDataBits));
             serial.setParity(static_cast<QSerialPort::Parity>(currentParity));
             serial.setStopBits(static_cast<QSerialPort::StopBits>(currentStopBits));
             serial.setFlowControl(static_cast<QSerialPort::FlowControl>(currentFlowControl));

             if (!serial.open(QIODevice::ReadWrite)) {
                 emit error(tr("Can't open %1, error code %2").arg(m_portName).arg(serial.error()));
                 return;
             }
         }

         if (serial.waitForReadyRead(currentWaitTimeout)) {
             // read request
             QByteArray requestData = serial.readAll();
             while (serial.waitForReadyRead(10)){
                 requestData += serial.readAll();
             }

             if(requestData.size() != 1){
                 continue;
             }

             if(requestData.at(0) != char(5)){
                 continue;
             }


             // write response
//             const QByteArray responseData = currentRespone.toUtf8();
             const char res = 6;
             const QByteArray responseData = QByteArray::fromRawData(&res, 1);
             serial.write(responseData);
             if (serial.waitForBytesWritten(m_waitTimeout)) {
                 const QString request = QString::fromUtf8(requestData);
                 emit this->request(request);
             } else {
                 emit timeout(tr("Wait write response timeout %1").arg(QTime::currentTime().toString()));
             }

             // emit this->request(request);

         } else {
              emit timeout(tr("Wait read request timeout %1").arg(QTime::currentTime().toString()));
         }

         m_mutex.lock();
         if (currentPortName != m_portName) {
             currentPortName = m_portName;
             currentPortNameChanged = true;
         } else {
             currentPortNameChanged = false;
         }
         currentWaitTimeout = m_waitTimeout;
         // currentRespone = m_response;
         m_mutex.unlock();
    }
}
