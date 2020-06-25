#include "mbedhosttestrunner.h"
#include <QSettings>
#include <QUuid>

#include "PrometheusEndlineTester_global.h"


namespace worlddirect {
  MbedHostTestRunner::MbedHostTestRunner(QObject* parent)
    :QSerialPort(parent),
      m_testRun("UNKONWN TEST RUN"),
      m_hostTest("UNKNOWN HOST TEST")
  {
    connect(this, &QSerialPort::readyRead, this, &MbedHostTestRunner::readSerialData);
    connect(this, &MbedHostTestRunner::newLineReceived, this, &MbedHostTestRunner::parseNewLine);
  }

  MbedHostTestRunner::~MbedHostTestRunner()
  {
  }

  void MbedHostTestRunner::openSerial()
  {
    if (isOpen() == true){
        return;
      }

    QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);
    auto port = settings.value(KEY_SERIAL_PORT).toString();
    auto baudRate = settings.value(KEY_SERIAL_BAUD).toInt();
    auto dataBits = settings.value(KEY_SERIAL_DATA).toInt();
    auto parity = settings.value(KEY_SERIAL_PARITY).toInt();
    auto stopBits = settings.value(KEY_SERIAL_STOP).toInt();
    auto flowControl = settings.value(KEY_SERIAL_FLOWCTRL).toInt();

    setPortName(port);
    setBaudRate(static_cast<QSerialPort::BaudRate>(baudRate));
    setDataBits(static_cast<QSerialPort::DataBits>(dataBits));
    setParity(static_cast<QSerialPort::Parity>(parity));
    setStopBits(static_cast<QSerialPort::StopBits>(stopBits));
    setFlowControl(static_cast<QSerialPort::FlowControl>(flowControl));

    if (open(QIODevice::ReadWrite) == false) {
        return;
      }
    emit serialOpened();
  }

  void MbedHostTestRunner::serialSendSync()
  {
    auto uuid = QUuid::createUuid();
    sendSync(uuid);
  }

  void MbedHostTestRunner::serialSendSyncUuid(const QUuid &uuid)
  {
    sendSync(uuid);
  }

  void MbedHostTestRunner::readSerialData()
  {
    while (canReadLine()) {
        auto dt = readLine().trimmed();
        emit newLineReceived(dt);
      }
  }

  void MbedHostTestRunner::parseNewLine(const QByteArray &dt)
  {
    if(dt.startsWith(GREENTEA_SETUP))
      {
        emit setupReceived();
      }

    if(dt.startsWith(GREENTEA_MSG_PREAMBLE))
      {
        parseMsg(dt);
      }

    if(dt.startsWith(GREENTEA_KV_PREAMBLE) && dt.endsWith(GREENTEA_KV_POSTAMBLE))
      {
        parseKv(dt);
      }

  }

  qint64 MbedHostTestRunner::sendString(const QByteArray &str)
  {
    if(isOpen() == false){
        return 1;
      }
    auto res = write(str);
    if(res < 0 ){
        return res;
      }
    auto cnt = res;

    res = write("\r\n");
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

  void MbedHostTestRunner::parseMsg(const QByteArray &line)
  {
    emit msgReceived(line);
  }

  void MbedHostTestRunner::parseKv(const QByteArray &line)
  {
    auto len = line.size();
    auto mid = line.mid(2, len-4);
    auto kv = mid.split(';');

    auto key = kv.first();
    kv.removeFirst();
    auto val = kv;
    checkForWellKnownKv(key, val);

  }

  void MbedHostTestRunner::checkForWellKnownKv(const QByteArray &key, const QList<QByteArray> &val)
  {

    // {{end;success}}
    if(key.compare(GREENTEA_TEST_ENV_END) == 0)
      {
        if(val.size()!= 1){
            return;
          }
        auto success = val[0].compare(GREENTEA_TEST_ENV_SUCCESS) == 0;
        emit endReceived(success);
        if(success){
            emit passedHostTestRun(m_testRun);
          }else{
            emit failedHostTestRun(m_testRun);
          }
      }

    // {{__exit;0}}
    if(key.compare(GREENTEA_TEST_ENV_EXIT) == 0)
      {
        if(val.size()!= 1){
            return;
          }
        bool ok = false;
        auto code = val[0].toInt(&ok);
        if(!ok){
            return;
          }
        emit exitReceived(code);
        return;
      }

    //{{__sync;66faacf8-b822-435c-b832-821de54306f2}}
    if(key.compare( GREENTEA_TEST_ENV_SYNC) == 0)
      {
        if(val.size()!= 1){
            return;
          }
        QUuid uuid(val[0]);
        if(uuid.isNull())
          {
            return;
          }
        emit syncReceived(uuid);
        newSyncReceived(uuid);
        return;
      }

    // {{__timeout;20}}
    if(key.compare( GREENTEA_TEST_ENV_TIMEOUT) == 0)
      {
        if(val.size()!= 1){
            return;
          }
        bool ok = false;
        auto sek = val[0].toInt(&ok);
        if(!ok){
            return;
          }
        emit timeoutReceived(sek);
        return;
      }

    // {{__host_test_name;default_auto}}
    if(key.compare( GREENTEA_TEST_ENV_HOST_TEST_NAME) == 0)
      {
        if(val.size()!= 1){
            return;
          }
        QString hostTestName(val[0]);
        newHostTestReceived(hostTestName);
        emit hostTestNameReceived(hostTestName);
        return;
      }

    // {{__version;1.3.0}}
    if(key.compare( GREENTEA_TEST_ENV_HOST_TEST_VERSION)  == 0)
      {
        if(val.size()!= 1){
            return;
          }
        QString version(val[0]);
        emit hostTestVersionReceived(version);
        return;
      }

    // {{__testcase_name;Simple Test}}
    if(key.compare( GREENTEA_TEST_ENV_TESTCASE_NAME) == 0)
      {
        if(val.size()!= 1){
            return;
          }
        QString testCaseName(val[0]);
        newTestCaseReceived(testCaseName);
        emit testCaseNameReceived(testCaseName);
        return;
      }

    // {{__testcase_count;2}}
    if(key.compare( GREENTEA_TEST_ENV_TESTCASE_COUNT) == 0)
      {
        if(val.size()!= 1){
            return;
          }
        bool ok = false;
        auto cnt = val[0].toInt(&ok);
        if(!ok){
            return;
          }
        emit testCaseCountReceived(cnt);
        return;
      }

    // {{__testcase_start;Simple Test}}
    if(key.compare( GREENTEA_TEST_ENV_TESTCASE_START) == 0)
      {
        if(val.size()!= 1){
            return;
          }
        QString testCaseName(val[0]);
        emit testCaseStartReeived(testCaseName);
        emit startTestCase(m_testRun, m_hostTest, testCaseName);
        return;
      }

    // {{__testcase_finish;Repeating Test;1;0}}
    if(key.compare( GREENTEA_TEST_ENV_TESTCASE_FINISH) == 0)
      {
        if(val.size()!= 3){
            return;
          }
        QString testCaseName(val[0]);
        bool ok = false;
        auto passed = val[1].toInt(&ok);
        if(!ok){
            return;
          }
        auto failed = val[2].toInt(&ok);
        if(!ok){
            return;
          }
        emit testCaseFinishReceived(testCaseName, passed, failed);
        if(failed == 0 )
          {
            emit passedTestCase(m_testRun, m_hostTest, testCaseName);
          }else {
            emit failedTestCase(m_testRun, m_hostTest, testCaseName);
          }
        return;
      }

    // {{__testcase_summary;2;0}}
    if(key.compare( GREENTEA_TEST_ENV_TESTCASE_SUMMARY) == 0)
      {
        if(val.size()!= 2){
            return;
          }
        bool ok = false;
        auto passed = val[0].toInt(&ok);
        if(!ok){
            return;
          }
        auto failed = val[1].toInt(&ok);
        if(!ok){
            return;
          }
        emit testCaseSummaryReceived(passed, failed);
        if(failed ==0){
            emit passedHostTest(m_testRun, m_hostTest);
          }else {
            emit failedHostTest(m_testRun, m_hostTest);
          }

        return;
      }

    // {{__type;prometheus}}
    if(key.compare(CO1_FUNCTIONAL_ENDLINE_TEST_TYPE) == 0)
      {
        if(val.size()!= 1){
            return;
          }
        QString type(val[0]);
        emit typeReceived(type);
        return;
      }

    // {{__hw_ver;1}}
    if(key.compare(CO1_FUNCTIONAL_ENDLINE_TEST_HWVER) == 0)
      {
        if(val.size()!= 1){
            return;
          }
        QString hwVer(val[0]);
        emit hardwareVersionReceived(hwVer);
        return;
      }

    // {{__ep_name;PO1ODA2NjI4UQgAIAA8}}
    if(key.compare(CO1_FUNCTIONAL_ENDLINE_TEST_EPNAME) == 0)
      {
        if(val.size()!= 1){
            return;
          }
        QString epName(val[0]);
        emit endpointNameReceived(epName);
        return;
      }

    // {{__icc_id;89423053217282084858}}
    if(key.compare(CO1_FUNCTIONAL_ENDLINE_TEST_ICCID) == 0)
      {
        if(val.size()!= 1){
            return;
          }
        QString iccId(val[0]);
        emit iccIdReceived(iccId);
        return;
      }

    emit kvReceived(key, val);
  }

  void MbedHostTestRunner::newHostTestReceived(const QString &hostTestName)
  {
    m_hostTest = hostTestName;
    emit newHostTest(m_testRun, m_hostTest);
  }

  void MbedHostTestRunner::newTestCaseReceived(const QString &testCaseName)
  {
    emit newTestCase(m_testRun, m_hostTest, testCaseName);
  }

  void MbedHostTestRunner::newSyncReceived(const QUuid &uuid)
  {
    m_testRun = uuid.toString(QUuid::WithoutBraces);
    emit newHostTestRun(m_testRun);
  }

  qint64 MbedHostTestRunner::sendKv(const QByteArray& key, const QByteArray& val)
  {
    if(isOpen() == false){
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

    res = write(key);
    if(res < 0 ){
        return res;
      }
    cnt += res;

    res = writeSeperator();
    if(res < 0 ){
        return res;
      }
    cnt += res;

    res = write(val);
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
    return write(GREENTEA_KV_PREAMBLE);
  }

  qint64 MbedHostTestRunner::writeSeperator()
  {
    return write(GREENTEA_KV_SEPERATOR);
  }

  qint64 MbedHostTestRunner::writePostamble()
  {
    return sendString(GREENTEA_KV_POSTAMBLE);
  }

}
