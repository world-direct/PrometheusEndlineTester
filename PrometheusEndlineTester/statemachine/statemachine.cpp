#include "statemachine.h"
#include <QSerialPort>

#include "ui/mainwindow.h"
#include "stlink/stlinkdeviceflasher.h"
#include <htrun/mbedhosttestrunner.h>
#include <provisioning/deviceprovisioning.h>

namespace worlddirect {

  StateMachine::StateMachine(QObject *parent)
    :QStateMachine(parent),
      m_ui(new MainWindow()),
      m_stflash(new StLinkDeviceFlasher()),
      m_htrun(new MbedHostTestRunner()),
      m_provisioning(new DeviceProvisioning())
  {
    connect(this, &StateMachine::printData,                     m_ui, &MainWindow::printData);

    connect(m_ui, &MainWindow::targetConnect,                   m_stflash, &StLinkDeviceFlasher::connectToTarget);
    connect(m_ui, &MainWindow::targetProgramTest,               m_stflash, &StLinkDeviceFlasher::programTargetTest);
    connect(m_ui, &MainWindow::targetReset,                     m_stflash, &StLinkDeviceFlasher::resetTarget);
    connect(m_ui, &MainWindow::serialConnect,                   m_htrun, &MbedHostTestRunner::openSerial);
    connect(m_ui, &MainWindow::serialSendSync,                  m_htrun, &MbedHostTestRunner::serialSendSync);
    connect(m_ui, &MainWindow::targetProgramFirmware,           m_stflash, &StLinkDeviceFlasher::programTargetFirmware);
    connect(m_ui, &MainWindow::requestToken,                    m_provisioning, &DeviceProvisioning::getToken);
    // connect(m_ui, &MainWindow::registerDevice,                  m_provisioning, &DeviceProvisioning::);
    // connect(m_ui, &MainWindow::getPsk,                          m_provisioning, &DeviceProvisioning::);
    // connect(m_ui, &MainWindow::validateEncryption,              m_provisioning, &DeviceProvisioning::);
    connect(m_ui, &MainWindow::downloadLatestFirmware,                     m_provisioning, &DeviceProvisioning::downloadLatestFirmware);

    connect(m_stflash, &StLinkDeviceFlasher::newLineReceived,   m_ui, &MainWindow::printData);
    connect(m_stflash, &StLinkDeviceFlasher::printData,         m_ui, &MainWindow::printData);

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

    connect(m_htrun, &MbedHostTestRunner::typeReceived,        m_ui, &MainWindow::typeReceived);
    connect(m_htrun, &MbedHostTestRunner::hardwareVersionReceived,        m_ui, &MainWindow::hardwareVersionReceived);
    connect(m_htrun, &MbedHostTestRunner::endpointNameReceived,        m_ui, &MainWindow::endpointNameReceived);
    connect(m_htrun, &MbedHostTestRunner::iccIdReceived,        m_ui, &MainWindow::iccIdReceived);

    connect(m_provisioning, &DeviceProvisioning::printData,         m_ui, &MainWindow::printData);

    createStates();
  }

  void worlddirect::StateMachine::createStates()
  {
    QState* initial = new QState();
    connect(initial, &QState::entered, m_ui, &MainWindow::readSettings );

    QState* startUi = new QState();
    connect(startUi, &QState::entered, m_ui, &MainWindow::show);

    QState* newTarget= new QState();
    connect(newTarget, &QState::entered, m_ui, &MainWindow::setNewTargetState);

    QState* targetNotConnected = new QState();
    connect(targetNotConnected, &QState::entered, m_ui, &MainWindow::setTargetNotConnectedState);

    QState* targetConnected = new QState();
    connect(targetConnected, &QState::entered, m_ui, &MainWindow::setTargetConnectedState);

    QState* targetNotProgrammed = new QState();
    connect(targetNotProgrammed, &QState::entered, m_ui, &MainWindow::setTargetNotProgrammedState);

    QState* targetProgrammed = new QState();
    connect(targetProgrammed, &QState::entered, m_ui, &MainWindow::setTargetProgrammedState);

    QState* targetNotReset = new QState();
    connect(targetNotReset, &QState::entered, m_ui, &MainWindow::setTargetNotResetState);

    QState* targetReset = new QState();
    connect(targetReset, &QState::entered, m_ui, &MainWindow::setTargetResetState);

    QState* serialNotConnected = new QState();
    connect(serialNotConnected, &QState::entered, m_ui, &MainWindow::setSerialNotConnectedState);

    QState* serialConnected = new QState();
    connect(serialConnected, &QState::entered, m_ui, &MainWindow::setSerialConnectedState);

    QState* serialNotSynced = new QState();
    connect(serialNotSynced, &QState::entered, m_ui, &MainWindow::setSerialNotSyncedState);

    QState* serialSynced = new QState();
    connect(serialSynced, &QState::entered, m_ui, &MainWindow::setSerialSyncedState);

    initial->addTransition(startUi);
    startUi->addTransition(newTarget);
    newTarget->addTransition(targetNotConnected);
    targetNotConnected->addTransition(m_stflash, &StLinkDeviceFlasher::targetConnected, targetConnected);
    targetConnected->addTransition(targetNotProgrammed);
    targetNotProgrammed->addTransition(m_stflash, &StLinkDeviceFlasher::targetProgrammed, targetProgrammed);
    targetProgrammed->addTransition(targetNotReset);
    targetNotReset->addTransition(m_stflash, &StLinkDeviceFlasher::targetReset, targetReset);
    targetReset->addTransition(serialNotConnected);
    serialNotConnected->addTransition(m_htrun, &MbedHostTestRunner::serialOpened, serialConnected);
    serialConnected->addTransition(serialNotSynced);
    //serialNotSynced->addTransition(m_htrun, &MbedHostTestRunner::ser)

    this->addState(initial);
    this->addState(startUi);
    this->addState(newTarget);
    this->addState(targetNotConnected);
    this->addState(targetConnected);
    this->addState(targetNotProgrammed);
    this->addState(targetProgrammed);
    this->addState(serialNotConnected);
    this->addState(serialConnected);
    this->addState(serialNotSynced);
    this->addState(serialSynced);

    this->setInitialState(initial);

  }

} // namespace worlddirect
