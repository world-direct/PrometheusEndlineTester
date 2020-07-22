#include "statemachine.h"
#include <QSerialPort>

#include "ui/mainwindow.h"
#include "stlink/stlinkdeviceflasher.h"
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
      m_deviceInformation(new DeviceInformation())
  {
    connect(this, &StateMachine::printData,                     m_ui, &MainWindow::printData);
    connect(this, &StateMachine::clearScreen,                     m_ui, &MainWindow::clearScreen);
    connect(this, &StateMachine::newTarget,                     m_deviceInformation, &DeviceInformation::clear);
    connect(this, &StateMachine::newTarget,                     m_htrun, &MbedHostTestRunner::close);

    connect(m_ui, &MainWindow::targetConnect,                   m_stflash, &StLinkDeviceFlasher::connectToTarget);
    connect(m_ui, &MainWindow::targetProgramTest,               m_stflash, &StLinkDeviceFlasher::programTargetTest);
    connect(m_ui, &MainWindow::targetReset,                     m_stflash, &StLinkDeviceFlasher::resetTarget);
    connect(m_ui, &MainWindow::serialConnect,                   m_htrun, &MbedHostTestRunner::openSerial);
    connect(
          m_ui, &MainWindow::serialSendSync,
          [this](){
        auto uuid = m_deviceInformation->uuid();
        m_htrun->serialSendSyncUuid(uuid);
      }
    );
    connect(
          m_ui, &MainWindow::serialSendPsk,
          [this](){
        auto psk = m_deviceInformation->psk();
        m_htrun->sendPSK(psk);
      }
    );
    connect(m_ui, &MainWindow::targetProgramFirmware,           m_stflash, &StLinkDeviceFlasher::programTargetFirmware);
    connect(m_ui, &MainWindow::requestToken,                    m_provisioning, &DeviceProvisioning::getToken);
    connect(
          m_ui, &MainWindow::registerDevice,
          [this](){
        auto epName = m_deviceInformation->endpointName();
        auto type = m_deviceInformation->type();
        auto hwVer = m_deviceInformation->hardwareVersion();
        auto iccId = m_deviceInformation->iccId();
        m_provisioning->createDevice(epName,type,hwVer, iccId);
      }
    );
    connect(
          m_ui, &MainWindow::getPsk,
          [this](){
        auto epName = m_deviceInformation->endpointName();
        m_provisioning->getPsk(epName);
      }
    );
    connect(
          m_ui, &MainWindow::validateEncryption,
          [this](){
        auto epName = m_deviceInformation->endpointName();
        auto iccId = m_deviceInformation->iccId();
        auto psk = m_deviceInformation->psk();
        m_provisioning->validatePsk(epName, iccId, psk);
      }
    );
    connect(m_ui, &MainWindow::downloadLatestFirmware,                     m_provisioning, &DeviceProvisioning::downloadLatestFirmware);
    connect(m_ui, &MainWindow::prepareTest, this, &StateMachine::error);
    connect(m_ui, &MainWindow::runTest, this, &StateMachine::success);
    connect(m_ui, &MainWindow::newTarget, this, &StateMachine::newTarget);
    connect(
          m_ui, &MainWindow::printNameplate,
          [this](){
        if(m_deviceInformation->valid() == false){
            this->errrorMessage("Error Nameplate is not valid");
            return ;
          }
        this->successMessage("Nameplat valid printing...");
        m_ui->nameplateValid();

      }
    );

    connect(m_stflash, &StLinkDeviceFlasher::newLineReceived,   m_ui, &MainWindow::printData);
    connect(m_stflash, &StLinkDeviceFlasher::printData,         m_ui, &MainWindow::printData);
    connect(m_stflash, &StLinkDeviceFlasher::successMessage,         this, & StateMachine::successMessage);
    connect(m_stflash, &StLinkDeviceFlasher::errorMessage,         this, & StateMachine::errrorMessage);

    connect(m_htrun, &MbedHostTestRunner::newLineReceived,      m_ui, &MainWindow::printData);
    connect(m_htrun, &MbedHostTestRunner::printData,            m_ui, &MainWindow::printData);
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
    connect(m_htrun, &MbedHostTestRunner::successMessage,         this, & StateMachine::successMessage);
    connect(m_htrun, &MbedHostTestRunner::errorMessage,         this, & StateMachine::errrorMessage);


    connect(m_deviceInformation, &DeviceInformation::typeChanged,        m_ui, &MainWindow::typeReceived);
    connect(m_deviceInformation, &DeviceInformation::hardwareVersionChanged,        m_ui, &MainWindow::hardwareVersionReceived);
    connect(m_deviceInformation, &DeviceInformation::endpointNameChanged    ,        m_ui, &MainWindow::endpointNameReceived);
    connect(m_deviceInformation, &DeviceInformation::iccIdChanged,        m_ui, &MainWindow::iccIdReceived);

    connect(m_provisioning, &DeviceProvisioning::printData,         m_ui, &MainWindow::printData);
    connect(m_provisioning, &DeviceProvisioning::successMessage,         this, & StateMachine::successMessage);
    connect(m_provisioning, &DeviceProvisioning::errorMessage,         this, & StateMachine::errrorMessage);
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
        emit clearScreen();
        emit printData("===Initial===");
        m_ui->readSettings();
        m_ui->show();
        m_deviceInformation->clear();
      }
    );

    QState* tokenRequested = new QState();
    tokenRequested->setObjectName(tr("Token requested"));
    connect(
          tokenRequested, &QState::entered,
          [this]() {
        emit clearScreen();
        emit printData("===Token requested===");
      }
    );

    QState* firmwareDownloaded= new QState();
    firmwareDownloaded->setObjectName(tr("firmwareDownloaded"));
    connect(
          firmwareDownloaded, &QState::entered,
          [this]() {
        emit clearScreen();
        emit printData("===Firmware Downloaded===");
      }
    );

    QState*  ready= new QState();
    ready->setObjectName(tr("ready"));
    connect(
          ready, &QState::entered,
          [this]() {
        emit clearScreen();
        emit printData("===READY===");
      }
    );

    QState* targetConnected = new QState();
    targetConnected->setObjectName(tr("targetConnected"));
    connect(
          targetConnected, &QState::entered,
          [this]() {
        emit clearScreen();
        emit printData("===Target Connected===");
      }
    );

    QState* targetTestProgrammed = new QState();
    targetTestProgrammed->setObjectName(tr("targetTestProgrammed"));
    connect(
          targetTestProgrammed, &QState::entered,
          [this]() {
        emit clearScreen();
        emit printData("===Target Test Programmed===");
      }
    );

    QState* targetTestReady = new QState();
    targetTestReady->setObjectName(tr("targetTestReady"));
    connect(
          targetTestReady, &QState::entered,
          [this]() {
        emit clearScreen();
        emit printData("===Target Test Ready===");
      }
    );

    QState* serialConnected = new QState();
    serialConnected->setObjectName(tr("serialConnected"));
    connect(
          serialConnected, &QState::entered,
          [this]() {
        emit clearScreen();
        emit printData("===Serial Connected===");
      }
    );

    QState* targetTestRunning = new QState();
    targetTestRunning->setObjectName(tr("targetTestRunning"));
    connect(
          targetTestRunning, &QState::entered,
          [this]() {
        emit clearScreen();
        emit printData("===Target Test Running===");
      }
    );

    QState* targetTested = new QState();
    targetTested->setObjectName(tr("targetTested"));
    connect(
          targetTested, &QState::entered,
          [this]() {
        emit clearScreen();
        emit printData("===Target Tested===");
      }
    );

    QState* targetNotPorvisioned = new QState();
    targetNotPorvisioned->setObjectName(tr("targetNotPorvisioned"));
    connect(
          targetNotPorvisioned, &QState::entered,
          [this]() {
        emit clearScreen();
        emit printData("===Target not Porvisioned===");
      }
    );

    QState* pskReceived = new QState();
    pskReceived->setObjectName(tr("pskReceived"));
    connect(
          pskReceived, &QState::entered,
          [this]() {
        emit clearScreen();
        emit printData("===PSK Received===");
      }
    );

    QState* pskVerified = new QState();
    pskVerified->setObjectName(tr("pskVerified"));
    connect(
          pskVerified, &QState::entered,
          [this]() {
        emit clearScreen();
        emit printData("===PSK Verified===");
      }
    );

    QState* targetProvisioned = new QState();
    targetProvisioned->setObjectName(tr("targetProvisioned"));
    connect(
          targetProvisioned, &QState::entered,
          [this]() {
        emit clearScreen();
        emit printData("===Target Provisioned===");
      }
    );

    QState* targetDone = new QState();
    targetDone->setObjectName(tr("targetDone"));
    connect(
          targetDone, &QState::entered,
          [this]() {
        emit clearScreen();
        emit printData("===Target Done===");
      }
    );

    initial->addTransition(this, &StateMachine::success, tokenRequested);
    initial->addTransition(this, &StateMachine::error, initial);

    tokenRequested->addTransition(this, &StateMachine::success, firmwareDownloaded);
    tokenRequested->addTransition(this, &StateMachine::error, tokenRequested);

    firmwareDownloaded->addTransition(this, &StateMachine::newTarget, ready);

    ready->addTransition(this, &StateMachine::success, targetConnected);
    ready->addTransition(this, &StateMachine::error, ready);

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

    targetTested->addTransition(this, &StateMachine::success, targetNotPorvisioned);
    targetTested->addTransition(this, &StateMachine::error, targetTested);
    targetTested->addTransition(this, &StateMachine::newTarget, ready);

    targetNotPorvisioned->addTransition(this, &StateMachine::success, pskReceived);
    targetNotPorvisioned->addTransition(this, &StateMachine::error, targetNotPorvisioned);
    targetNotPorvisioned->addTransition(this, &StateMachine::newTarget, ready);

    pskReceived->addTransition(this, &StateMachine::success, pskVerified);
    pskReceived->addTransition(this, &StateMachine::error, pskReceived);
    pskReceived->addTransition(this, &StateMachine::newTarget, ready);

    pskVerified->addTransition(this, &StateMachine::success, targetProvisioned);
    pskVerified->addTransition(this, &StateMachine::error, pskVerified);
    pskVerified->addTransition(this, &StateMachine::newTarget, ready);

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
    this->addState(targetNotPorvisioned);
    this->addState(pskReceived);
    this->addState(pskVerified);
    this->addState(targetProvisioned);
    this->addState(targetDone);

    this->setInitialState(initial);

  }

  void StateMachine::successMessage(const QString &msg)
  {
    printData(msg.toLocal8Bit());
    emit success();
  }

  void StateMachine::errrorMessage(const QString &msg)
  {
    printData(msg.toLocal8Bit());
    emit error();
  }

} // namespace worlddirect
