#include "statemachine.h"
#include <QSerialPort>

#include "ui/mainwindow.h"
#include "stlink/stlinkdeviceflasher.h"
#include <QThread>
#include <deviceinformation/deviceinformation.h>
#include <htrun/mbedhosttestrunner.h>
#include <provisioning/deviceprovisioning.h>

namespace worlddirect {

  StateMachine::StateMachine(QObject *parent)
    :QStateMachine(parent),
      m_ui(new MainWindow()),
      m_stflash(new StLinkDeviceFlasher()),
      m_htrun(new MbedHostTestRunner()),
      m_provisioning(new DeviceProvisioning()),
      m_deviceInformation(new DeviceInformation()),
      m_autorun(true)
  {
    connect(this, &StateMachine::printData,                     m_ui, &MainWindow::printData);
    connect(this, &StateMachine::clearScreen,                   m_ui, &MainWindow::clearScreen);
    connect(this, &StateMachine::requestToken,                  m_provisioning, &DeviceProvisioning::getToken);
    connect(this, &StateMachine::downloadFirmware,              m_provisioning, &DeviceProvisioning::downloadLatestFirmware);
    connect(this, &StateMachine::newTarget,                     m_deviceInformation, &DeviceInformation::clear);
    connect(this, &StateMachine::newTarget,                     m_htrun, &MbedHostTestRunner::close);
    connect(this, &StateMachine::conncectProgrammer,            m_stflash, &StLinkDeviceFlasher::connectToTarget);
    connect(this, &StateMachine::programTest,                   m_stflash, &StLinkDeviceFlasher::programTargetTest);
    connect(this, &StateMachine::resetTarget,                   m_stflash, &StLinkDeviceFlasher::resetTarget);
    connect(this, &StateMachine::connectSerial,                 m_htrun, &MbedHostTestRunner::openSerial);
    connect(
          this, &StateMachine::sendSync,
          [this](){
        auto uuid = m_deviceInformation->uuid();
        m_htrun->serialSendSyncUuid(uuid);
      }
    );
    connect(this, &StateMachine::programFirmware,           m_stflash, &StLinkDeviceFlasher::programTargetFirmware);
    connect(
          this, &StateMachine::registerDevice,
          [this](){
        auto epName = m_deviceInformation->endpointName();
        auto type = m_deviceInformation->type();
        auto hwVer = m_deviceInformation->hardwareVersion();
        auto iccId = m_deviceInformation->iccId();
        m_provisioning->createDevice(epName,type,hwVer, iccId);
      }
    );
    connect(
          this, &StateMachine::getPsk,
          [this](){
        auto epName = m_deviceInformation->endpointName();
        m_provisioning->getPsk(epName);
      }
    );
    connect(
          this, &StateMachine::validateEncryption,
          [this](){
        auto epName = m_deviceInformation->endpointName();
        auto iccId = m_deviceInformation->iccId();
        auto psk = m_deviceInformation->psk();
        m_provisioning->validatePsk(epName, iccId, psk);
      }
    );
    connect(
          this, &StateMachine::sendPsk,
          [this](){
        auto psk = m_deviceInformation->psk();
        m_htrun->sendPSK(psk);
      }
    );
    connect(
          this, &StateMachine::printNameplate,
          [this](){
        if(m_deviceInformation->valid() == false){
            this->errrorMessage("Error Nameplate is not valid");
            return ;
          }
        this->successMessage("Nameplate valid: printing...");
        m_ui->nameplateValid();

      }
    );
    connect(
          this, &StateMachine::runTest,
          [this](){
        m_autorun = true;
        //emit newTarget();//this is dirty: the statmachine reenters the state on error...
      }
    );

    connect(m_ui, &MainWindow::downloadLatestFirmware,          this, &StateMachine::downloadFirmware);
    connect(m_ui, &MainWindow::requestToken,                    this, &StateMachine::requestToken);
    connect(m_ui, &MainWindow::newTarget,                       this, &StateMachine::newTarget);
    connect(m_ui, &MainWindow::targetConnect,                   this, &StateMachine::conncectProgrammer);
    connect(m_ui, &MainWindow::targetProgramTest,               this, &StateMachine::programTest);
    connect(m_ui, &MainWindow::targetReset,                     this, &StateMachine::resetTarget);
    connect(m_ui, &MainWindow::serialConnect,                   this, &StateMachine::connectSerial);
    connect(m_ui, &MainWindow::serialSendSync,                  this, &StateMachine::sendSync);
    connect(m_ui, &MainWindow::targetProgramFirmware,           this, &StateMachine::programFirmware);
    connect(m_ui, &MainWindow::registerDevice,                  this, &StateMachine::registerDevice);
    connect(m_ui, &MainWindow::getPsk,                          this, &StateMachine::getPsk);
    connect(m_ui, &MainWindow::validateEncryption,              this, &StateMachine::validateEncryption);
    connect(m_ui, &MainWindow::serialSendPsk,                   this, &StateMachine::sendPsk);
    connect(m_ui, &MainWindow::printNameplate,                  this, &StateMachine::printNameplate);
    connect(m_ui, &MainWindow::runTest,                         this, &StateMachine::runTest);
    connect(m_ui, &MainWindow::skip,                            this, &StateMachine::success);

    connect(m_stflash, &StLinkDeviceFlasher::newLineReceived,   this, &StateMachine::printData);
    connect(m_stflash, &StLinkDeviceFlasher::printData,         this, &StateMachine::printData);
    connect(m_stflash, &StLinkDeviceFlasher::successMessage,    this, & StateMachine::successMessage);
    connect(m_stflash, &StLinkDeviceFlasher::errorMessage,      this, & StateMachine::errrorMessage);

    connect(m_htrun, &MbedHostTestRunner::newLineReceived,      this, &StateMachine::printData);
    connect(m_htrun, &MbedHostTestRunner::printData,            this, &StateMachine::printData);
    connect(m_htrun, &MbedHostTestRunner::successMessage,       this, & StateMachine::successMessage);
    connect(m_htrun, &MbedHostTestRunner::errorMessage,         this, & StateMachine::errrorMessage);
    connect(m_htrun, &MbedHostTestRunner::newHostTestRun,       m_ui, &MainWindow::newHostTestRun);
    connect(m_htrun, &MbedHostTestRunner::newHostTest,          m_ui, &MainWindow::newHostTest);
    connect(m_htrun, &MbedHostTestRunner::newTestCase,          m_ui, &MainWindow::newTestCase);
    connect(m_htrun, &MbedHostTestRunner::startTestCase,        m_ui, &MainWindow::startTestCase);
    connect(m_htrun, &MbedHostTestRunner::passedTestCase,        m_ui, &MainWindow::passedTestCase);
    connect(m_htrun, &MbedHostTestRunner::failedTestCase,        m_ui, &MainWindow::failedTestCase);
    connect(m_htrun, &MbedHostTestRunner::passedHostTest,        m_ui, &MainWindow::passedHostTest);
    connect(m_htrun, &MbedHostTestRunner::failedHostTest,        m_ui, &MainWindow::failedHostTest);
    connect(m_htrun, &MbedHostTestRunner::passedHostTestRun,        m_ui, &MainWindow::passedHostTestRun);
    connect(m_htrun, &MbedHostTestRunner::failedHostTestRun,        m_ui, &MainWindow::failedHostTestRun);
    connect(m_htrun, &MbedHostTestRunner::typeReceived,                 m_deviceInformation, &DeviceInformation::setType);
    connect(m_htrun, &MbedHostTestRunner::hardwareVersionReceived,      m_deviceInformation, &DeviceInformation::setHardwareVersion);
    connect(m_htrun, &MbedHostTestRunner::endpointNameReceived,         m_deviceInformation, &DeviceInformation::setEndpointName);
    connect(m_htrun, &MbedHostTestRunner::iccIdReceived,                m_deviceInformation, &DeviceInformation::setIccId);
    connect(m_htrun, &MbedHostTestRunner::firmwareStarted,        this, &StateMachine::success);


    connect(m_deviceInformation, &DeviceInformation::typeChanged,        m_ui, &MainWindow::typeReceived);
    connect(m_deviceInformation, &DeviceInformation::hardwareVersionChanged,        m_ui, &MainWindow::hardwareVersionReceived);
    connect(m_deviceInformation, &DeviceInformation::endpointNameChanged    ,        m_ui, &MainWindow::endpointNameReceived);
    connect(m_deviceInformation, &DeviceInformation::iccIdChanged,        m_ui, &MainWindow::iccIdReceived);

    connect(m_provisioning, &DeviceProvisioning::printData,         this, &StateMachine::printData);
    connect(m_provisioning, &DeviceProvisioning::successMessage,    this, & StateMachine::successMessage);
    connect(m_provisioning, &DeviceProvisioning::errorMessage,      this, & StateMachine::errrorMessage);
    connect(m_provisioning, &DeviceProvisioning::pskReceived,       m_deviceInformation, &DeviceInformation::setPSK);

    createStates();
  }

  void StateMachine::createStates()
  {
    QState* initial = new QState();
    initial->setObjectName(tr("Intitial State"));
    connect(
          initial, &QState::entered,
          [this]() {
        emit printData("===Initial===");
        m_ui->readSettings();
        m_ui->show();
        m_deviceInformation->clear();
        if(m_autorun){
            //emit requestToken();
            QTimer::singleShot(1000, this, &StateMachine::requestToken);
          }
      }
    );

    QState* tokenRequested = new QState();
    tokenRequested->setObjectName(tr("Token requested"));
    connect(
          tokenRequested, &QState::entered,
          [this]() {
        emit printData("===Token requested===");
        if(m_autorun){
            //emit downloadFirmware();
            QTimer::singleShot(1000, this, &StateMachine::downloadFirmware);
          }
      }
    );

    QState* firmwareDownloaded= new QState();
    firmwareDownloaded->setObjectName(tr("firmwareDownloaded"));
    connect(
          firmwareDownloaded, &QState::entered,
          [this]() {
        emit printData("===Firmware Downloaded===");
        if(m_autorun){
            //emit newTarget();
            QTimer::singleShot(1000, this, &StateMachine::newTarget);
          }
      }
    );

    QState*  ready= new QState();
    ready->setObjectName(tr("ready"));
    connect(
          ready, &QState::entered,
          [this]() {
        emit clearScreen();
        emit printData("===READY===");
        if(m_autorun){
            //emit conncectProgrammer();
            QTimer::singleShot(1000, this, &StateMachine::conncectProgrammer);
          }
      }
    );

    QState* targetConnected = new QState();
    targetConnected->setObjectName(tr("targetConnected"));
    connect(
          targetConnected, &QState::entered,
          [this]() {
        emit printData("===Target Connected===");
        if(m_autorun){
            //emit programTest();
            QTimer::singleShot(1000, this, &StateMachine::programTest);
          }
      }
    );

    QState* targetTestProgrammed = new QState();
    targetTestProgrammed->setObjectName(tr("targetTestProgrammed"));
    connect(
          targetTestProgrammed, &QState::entered,
          [this]() {
        emit printData("===Target Test Programmed===");
        if(m_autorun){
            //emit resetTarget();
            QTimer::singleShot(1000, this, &StateMachine::resetTarget);
          }
      }
    );

    QState* targetTestReady = new QState();
    targetTestReady->setObjectName(tr("targetTestReady"));
    connect(
          targetTestReady, &QState::entered,
          [this]() {
        emit printData("===Target Test Ready===");
        if(m_autorun){
            //emit connectSerial();
            QTimer::singleShot(1000, this, &StateMachine::connectSerial);
          }
      }
    );

    QState* serialConnected = new QState();
    serialConnected->setObjectName(tr("serialConnected"));
    connect(
          serialConnected, &QState::entered,
          [this]() {
        emit printData("===Serial Connected===");
        if(m_autorun){
            //emit sendSync();
            QTimer::singleShot(1000, this, &StateMachine::sendSync);
          }
      }
    );

    QState* targetTestRunning = new QState();
    targetTestRunning->setObjectName(tr("targetTestRunning"));
    connect(
          targetTestRunning, &QState::entered,
          [this]() {
        emit printData("===Target Test Running===");
      }
    );

    QState* targetTested = new QState();
    targetTested->setObjectName(tr("targetTested"));
    connect(
          targetTested, &QState::entered,
          [this]() {
        emit printData("===Target Tested===");
        if(m_autorun){
            //emit programFirmware();
            QTimer::singleShot(1000, this, &StateMachine::programFirmware);
          }
      }
    );

    QState* targetNotRegistered = new QState();
    targetNotRegistered->setObjectName(tr("targetNotRegistered"));
    connect(
          targetNotRegistered, &QState::entered,
          [this]() {
        emit printData("===Target not Registered===");
        if(m_autorun){
            //emit registerDevice();
            QTimer::singleShot(1000, this, &StateMachine::registerDevice);
          }
      }
    );


    QState* targetNotPorvisioned = new QState();
    targetNotPorvisioned->setObjectName(tr("targetNotPorvisioned"));
    connect(
          targetNotPorvisioned, &QState::entered,
          [this]() {
        emit printData("===Target not Porvisioned===");
        if(m_autorun){
            //emit getPsk();
            QTimer::singleShot(1000, this, &StateMachine::getPsk);
          }
      }
    );

    QState* pskReceived = new QState();
    pskReceived->setObjectName(tr("pskReceived"));
    connect(
          pskReceived, &QState::entered,
          [this]() {
        emit printData("===PSK Received===");
        if(m_autorun){
            //emit validateEncryption();
            QTimer::singleShot(1000, this, &StateMachine::validateEncryption);
          }
      }
    );

    QState* pskVerified = new QState();
    pskVerified->setObjectName(tr("pskVerified"));
    connect(
          pskVerified, &QState::entered,
          [this]() {
        emit printData("===PSK Verified===");
        if(m_autorun){
            //emit sendPsk();
            QTimer::singleShot(1000, this, &StateMachine::resetTarget);
          }
      }
    );

    QState* targetFirmwareReady = new QState();
    targetFirmwareReady->setObjectName(tr("targetFirmwareReady"));
    connect(
          targetFirmwareReady, &QState::entered,
          [this]() {
        emit printData("===Target Firmware Ready===");
        if(m_autorun){
            //emit sendPsk();
            QTimer::singleShot(1000, this, &StateMachine::sendPsk);
          }
      }
    );

    QState* targetProvisioned = new QState();
    targetProvisioned->setObjectName(tr("targetProvisioned"));
    connect(
          targetProvisioned, &QState::entered,
          [this]() {
        emit printData("===Target Provisioned===");
        if(m_autorun){
        m_htrun->close();
            //emit printNameplate();
            QTimer::singleShot(1000, m_htrun, &MbedHostTestRunner::close);
            QTimer::singleShot(1000, this, &StateMachine::printNameplate);
          }
      }
    );

    QState* targetDone = new QState();
    targetDone->setObjectName(tr("targetDone"));
    connect(
          targetDone, &QState::entered,
          [this]() {
        emit printData("===Target Done===");
        m_autorun = false;
      }
    );

    initial->addTransition(this, &StateMachine::success, tokenRequested);
    initial->addTransition(this, &StateMachine::error, initial);

    tokenRequested->addTransition(this, &StateMachine::success, firmwareDownloaded);
    tokenRequested->addTransition(this, &StateMachine::error, tokenRequested);

    firmwareDownloaded->addTransition(this, &StateMachine::newTarget, ready);

    ready->addTransition(this, &StateMachine::success, targetConnected);
    ready->addTransition(this, &StateMachine::error, ready);
    ready->addTransition(this, &StateMachine::newTarget, ready);

    targetConnected->addTransition(this, &StateMachine::success, targetTestProgrammed);
    targetConnected->addTransition(this, &StateMachine::error, targetConnected);
    targetConnected->addTransition(this, &StateMachine::newTarget, ready);

    targetTestProgrammed->addTransition(this, &StateMachine::success, targetTestReady);
    targetTestProgrammed->addTransition(this, &StateMachine::error, targetTestProgrammed);
    targetTestProgrammed->addTransition(this, &StateMachine::newTarget, ready);

    targetTestReady->addTransition(this, &StateMachine::success, serialConnected);
    targetTestReady->addTransition(this, &StateMachine::error, targetTestReady);
    targetTestReady->addTransition(this, &StateMachine::newTarget, ready);

    serialConnected->addTransition(this, &StateMachine::success, targetTestRunning);
    serialConnected->addTransition(this, &StateMachine::error, serialConnected);
    serialConnected->addTransition(this, &StateMachine::newTarget, ready);

    targetTestRunning->addTransition(this, &StateMachine::success, targetTested);
    targetTestRunning->addTransition(this, &StateMachine::error, targetTestRunning);
    targetTestRunning->addTransition(this, &StateMachine::newTarget, ready);

    targetTested->addTransition(this, &StateMachine::success, targetNotRegistered);
    targetTested->addTransition(this, &StateMachine::error, targetTested);
    targetTested->addTransition(this, &StateMachine::newTarget, ready);

    targetNotRegistered->addTransition(this, &StateMachine::success, targetNotPorvisioned);
    targetNotRegistered->addTransition(this, &StateMachine::error, targetNotRegistered);
    targetNotRegistered->addTransition(this, &StateMachine::newTarget, ready);

    targetNotPorvisioned->addTransition(this, &StateMachine::success, pskReceived);
    targetNotPorvisioned->addTransition(this, &StateMachine::error, targetNotPorvisioned);
    targetNotPorvisioned->addTransition(this, &StateMachine::newTarget, ready);

    pskReceived->addTransition(this, &StateMachine::success, pskVerified);
    pskReceived->addTransition(this, &StateMachine::error, pskReceived);
    pskReceived->addTransition(this, &StateMachine::newTarget, ready);

    pskVerified->addTransition(this, &StateMachine::success, targetFirmwareReady);
    pskVerified->addTransition(this, &StateMachine::error, pskVerified);
    pskVerified->addTransition(this, &StateMachine::newTarget, ready);

    targetFirmwareReady->addTransition(this, &StateMachine::success, targetProvisioned);
    targetFirmwareReady->addTransition(this, &StateMachine::error, targetFirmwareReady);
    targetFirmwareReady->addTransition(this, &StateMachine::newTarget, ready);

    targetProvisioned->addTransition(this, &StateMachine::success, targetDone);
    targetProvisioned->addTransition(this, &StateMachine::error, targetProvisioned);
    targetProvisioned->addTransition(this, &StateMachine::newTarget, ready);

    targetDone->addTransition(this, &StateMachine::newTarget, ready);

    this->addState(initial);
    this->addState(tokenRequested);
    this->addState(firmwareDownloaded);
    this->addState(ready);
    this->addState(targetConnected);
    this->addState(targetTestProgrammed);
    this->addState(targetTestReady);
    this->addState(serialConnected);
    this->addState(targetTestRunning);
    this->addState(targetTested);
    this->addState(targetNotRegistered);
    this->addState(targetNotPorvisioned);
    this->addState(pskReceived);
    this->addState(pskVerified);
    this->addState(targetFirmwareReady);
    this->addState(targetProvisioned);
    this->addState(targetDone);

    this->setInitialState(initial);

  }

  void StateMachine::successMessage(const QString &msg)
  {
    emit clearScreen();
    printData(msg.toLocal8Bit());
    //emit success();
    QTimer::singleShot(100, this, &StateMachine::success);

  }

  void StateMachine::errrorMessage(const QString &msg)
  {
    m_autorun = false;
    printData(msg.toLocal8Bit());
    //emit error();
    QTimer::singleShot(100, this, &StateMachine::error);
  }

} // namespace worlddirect
