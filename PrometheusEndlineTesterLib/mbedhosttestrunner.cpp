#include "mbedhosttestrunner.h"
#include<QSerialPort>
#include <QUuid>

namespace worlddirect {
  MbedHostTestRunner::MbedHostTestRunner(QObject* parent)
    :QObject(parent),
      m_serial(nullptr)
  {

  }

  MbedHostTestRunner::~MbedHostTestRunner()
  {
    disconnect();
  }

  bool MbedHostTestRunner::isConnected() const
  {
    if(m_serial == nullptr){
        return false;
      }
    return m_serial->isOpen();
  }

  int MbedHostTestRunner::connect(QString port, int baudRate, int dataBits, int parity, int stopBits, int flowControl)
  {
    if(m_serial != nullptr){
        return 1;
      }
    m_serial = new QSerialPort(this);
    m_serial->setPortName(port);
    m_serial->setBaudRate(static_cast<QSerialPort::BaudRate>(baudRate));
    m_serial->setDataBits(static_cast<QSerialPort::DataBits>(dataBits));
    m_serial->setParity(static_cast<QSerialPort::Parity>(parity));
    m_serial->setStopBits(static_cast<QSerialPort::StopBits>(stopBits));
    m_serial->setFlowControl(static_cast<QSerialPort::FlowControl>(flowControl));

    if (m_serial->open(QIODevice::ReadWrite) == false) {
        qWarning("could not connect to serial: %s",  qUtf8Printable(m_serial->errorString().toLatin1()) );
        delete m_serial;
        m_serial = nullptr;
        return 2;
      }

    return 0;
  }

  int MbedHostTestRunner::disconnect()
  {
    if(isConnected() == false){
        return 1;
      }

    m_serial->close();
    delete m_serial;
    m_serial = nullptr;
    return 0;
  }

  qint64 MbedHostTestRunner::sendString(QByteArray str)
  {
    if(isConnected() == false){
        return 1;
      }
    auto res = writeString(str);
    if(res < 0 ){
        return res;
      }
    auto cnt = res;

    res = writeString("\r\n");
    if(res < 0 ){
        return res;
      }
    cnt += res;

    return cnt;
  }

  qint64 MbedHostTestRunner::sendSync(const QUuid &uuid)
  {
    return sendSync(uuid.toByteArray(QUuid::WithoutBraces));
  }

  qint64 MbedHostTestRunner::sendSync(const QByteArray &uuid)
  {
    return sendKv(GREENTEA_TEST_ENV_SYNC, uuid);
  }

  qint64 MbedHostTestRunner::sendKv(const QByteArray& key, const QByteArray& val)
  {
    if(isConnected() == false){
        return -1;
      }

    if (key.isEmpty() || val.isEmpty()) {
        return -2;
      }

    auto res = writePreamble();
    if(res < 0 ){
        return res;
      }
    auto cnt = res;

    res = writeString(key);
    if(res < 0 ){
        return res;
      }
    cnt += res;

    res = writeSeperator();
    if(res < 0 ){
        return res;
      }
    cnt += res;

    res = writeString(val);
    if(res < 0 ){
        return res;
      }
    cnt += res;

    res = writePostamble();
    if(res < 0 ){
        return res;
      }
    cnt += res;

    return cnt;
  }


  qint64 MbedHostTestRunner::writePreamble()
  {
    return writeString(GREENTEA_KV_PREAMBLE);
  }

  qint64 MbedHostTestRunner::writeString(const QByteArray& str)
  {
    return m_serial->write(str);
  }

  qint64 MbedHostTestRunner::writeSeperator()
  {
    return writeString(GREENTEA_KV_SEPERATOR);
  }

  qint64 MbedHostTestRunner::writePostamble()
  {
    return sendString(GREENTEA_KV_POSTAMBLE);
  }

}
