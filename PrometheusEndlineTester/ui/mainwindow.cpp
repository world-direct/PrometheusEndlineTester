#include "mainwindow.h"

#include <QAction>
#include <QGridLayout>
#include <QSettings>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QToolBar>
#include <QStateMachine>
#include <QProcess>
#include <QUuid>

#include <ui/widgets/console.h>
#include <ui/widgets/testexplorer.h>

#include "ui/dialogs/settingsdialog.h"

#include "PrometheusEndlineTester_global.h"



namespace worlddirect{

  MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_centralWidget(new QWidget(this)),
      m_console(new Console),
      m_testExplorer(new TestExplorer()),
      m_settingsDialog(new SettingsDialog(this)),
      m_newAct(Q_NULLPTR),
      m_openAct(Q_NULLPTR),
      m_saveAct(Q_NULLPTR),
      m_saveAsAct(Q_NULLPTR),
      m_exitAct(Q_NULLPTR),
      m_aboutAct(Q_NULLPTR),
      m_connectAct(Q_NULLPTR),
      m_programTSTAct(Q_NULLPTR),
      m_resetAct(Q_NULLPTR),
      m_serialConnectAct(Q_NULLPTR),
      m_serialSendSyncAct(Q_NULLPTR),
      m_serialSendPskAct(Q_NULLPTR),
      m_printNameplateAct(Q_NULLPTR),
      m_programFWAct(Q_NULLPTR),
      m_requestTokenAct(Q_NULLPTR),
      m_registerDeviceAct(Q_NULLPTR),
      m_getPskAct(Q_NULLPTR),
      m_validateEncryptionAct(Q_NULLPTR),
      m_getLatestFwAct(Q_NULLPTR),
      m_runTestAct(Q_NULLPTR),
      m_skipStepAct(Q_NULLPTR),
      m_fileMenu(Q_NULLPTR),
      m_targetMenu(Q_NULLPTR),
      m_serialMenu(Q_NULLPTR),
      m_provisioningMenu(Q_NULLPTR),
      m_testMenu(Q_NULLPTR),
      m_helpMenu(Q_NULLPTR),
      m_fileToolBar(Q_NULLPTR),
      m_targetToolBar(Q_NULLPTR),
      m_serialToolBar(Q_NULLPTR),
      m_provisioningToolBar(Q_NULLPTR),
      m_testToolBar(Q_NULLPTR)
  {
    setContentsMargins(0,0,0,0);
    setWindowTitle(PROJECT_NAME);

    auto m_layout = new QGridLayout();
    m_layout->setMargin(0);
    m_layout->setHorizontalSpacing(0);
    m_layout->setVerticalSpacing(0);
    m_centralWidget->setLayout(m_layout);

    setCentralWidget(m_centralWidget);

    m_console->sizePolicy().setHorizontalStretch(5);
    m_testExplorer->sizePolicy().setHorizontalStretch(1);

    m_layout->addWidget(m_console,0,0);
    m_layout->addWidget(m_testExplorer,0,1);

    connect(this, &MainWindow::printData, m_console, &Console::printData);
    connect(this, &MainWindow::clearScreen, m_console, &Console::clear);
    connect(this, &MainWindow::newTarget, m_testExplorer, &TestExplorer::clear);

    m_testExplorer->clear();

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();


  }

  void MainWindow::readSettings()
  {
    auto settings = m_settingsDialog->settings();
    restoreGeometry(settings.value(KEY_UI_GEOMETRY).toByteArray());
    restoreState(settings.value(KEY_UI_WINDOWSTATE).toByteArray());
  }


  void MainWindow::newHostTestRun(const QString &hostTestRun)
  {
    m_testExplorer->newHostTestRun(hostTestRun);
  }

  void MainWindow::newHostTest(const QString &hostTestRun, const QString &hostTest)
  {
    m_testExplorer->newHostTest(hostTestRun, hostTest);
  }

  void MainWindow::newTestCase(const QString &hostTestRun, const QString &hostTest, const QString &hostTestCase)
  {
    m_testExplorer->newTestCase(hostTestRun, hostTest, hostTestCase);
  }

  void MainWindow::startTestCase(const QString &hostTestRun, const QString &hostTest, const QString &hostTestCase)
  {
    m_testExplorer->startTestCase(hostTestRun, hostTest, hostTestCase);
  }

  void MainWindow::passedTestCase(const QString &hostTestRun, const QString &hostTest, const QString &hostTestCase)
  {
    m_testExplorer->passedTestCase(hostTestRun, hostTest, hostTestCase);
  }

  void MainWindow::failedTestCase(const QString &hostTestRun, const QString &hostTest, const QString &hostTestCase)
  {
    m_testExplorer->failedTestCase(hostTestRun, hostTest, hostTestCase);
  }

  void MainWindow::passedHostTest(const QString &hostTestRun, const QString &hostTest)
  {
    m_testExplorer->passedHostTest(hostTestRun, hostTest);
  }

  void MainWindow::failedHostTest(const QString &hostTestRun, const QString &hostTest)
  {
    m_testExplorer->failedHostTest(hostTestRun, hostTest);
  }

  void MainWindow::passedHostTestRun(const QString &hostTestRun)
  {
    m_testExplorer->passedHostTestRun(hostTestRun);
  }

  void MainWindow::failedHostTestRun(const QString &hostTestRun)
  {
    m_testExplorer->failedHostTestRun(hostTestRun);
  }

  void MainWindow::typeReceived(const QString &type)
  {
    m_testExplorer->typeReceived(type);
  }

  void MainWindow::hardwareVersionReceived(const QString &hwVer)
  {
    m_testExplorer->hardwareVersionReceived(hwVer);
  }

  void MainWindow::endpointNameReceived(const QString &epName)
  {
    m_testExplorer->endpointNameReceived(epName);
  }

  void MainWindow::iccIdReceived(const QString &iccId)
  {
    m_testExplorer->iccIdReceived(iccId);
  }

  void MainWindow::nameplateValid()
  {
   m_testExplorer->printNameplate();
  }

  void MainWindow::createActions()
  {
    m_newAct = new QAction(tr("&New"), this);
    m_newAct->setShortcuts(QKeySequence::New);
    m_newAct->setStatusTip(tr("Create a new file"));
    connect(m_newAct, &QAction::triggered, this, &MainWindow::newTarget);

//    m_openAct = new QAction(tr("&Open..."), this);
//    m_openAct->setShortcuts(QKeySequence::Open);
//    m_openAct->setStatusTip(tr("Open an existing file"));
//    connect(m_openAct, &QAction::triggered, this, &MainWindow::open);
//    m_openAct->setDisabled(true);

//    m_saveAct = new QAction(tr("&Save"), this);
//    m_saveAct->setShortcuts(QKeySequence::Save);
//    m_saveAct->setStatusTip(tr("Save the document to disk"));
//    connect(m_saveAct, &QAction::triggered, this, &MainWindow::save);
//    m_saveAct->setDisabled(true);

//    m_saveAsAct = new QAction(tr("Save &As..."), this);
//    m_saveAsAct->setShortcuts(QKeySequence::SaveAs);
//    m_saveAsAct->setStatusTip(tr("Save the document under a new name"));
//    connect(m_saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);

    m_exitAct = new QAction(tr("E&xit"), this);
    m_exitAct->setShortcuts(QKeySequence::Quit);
    m_exitAct->setStatusTip(tr("Exit the application"));
    connect(m_exitAct, &QAction::triggered, this, &MainWindow::close);

    m_aboutAct = new QAction(tr("&About"), this);
    m_aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(m_aboutAct, &QAction::triggered, this, &MainWindow::about);

    m_connectAct = new QAction(tr("Connect"), this);
    m_connectAct->setStatusTip(tr("Connect to the target"));
    connect(m_connectAct, &QAction::triggered, this, &MainWindow::targetConnect);

    m_programTSTAct = new QAction(tr("Program Test"), this);
    m_programTSTAct->setStatusTip(tr("Program & Verify Test"));
    connect(m_programTSTAct, &QAction::triggered, this, &MainWindow::targetProgramTest);

    m_resetAct = new QAction(tr("Reset"));
    m_resetAct->setStatusTip(tr("reset taget"));
    connect(m_resetAct, &QAction::triggered, this, &MainWindow::targetReset);

    m_serialConnectAct = new QAction(tr("Connect Serial"));
    m_serialConnectAct->setStatusTip(tr("Connect to the target"));
    connect(m_serialConnectAct, &QAction::triggered, this, &MainWindow::serialConnect);

    m_serialSendSyncAct = new QAction(tr("Send Sync"), this);
    m_serialSendSyncAct->setStatusTip(tr("send sync message"));
    connect(m_serialSendSyncAct, &QAction::triggered, this, &MainWindow::serialSendSync);

    m_serialSendPskAct = new QAction(tr("Send PSK"), this);
    m_serialSendPskAct->setStatusTip(tr("send psk to device"));
    connect(m_serialSendPskAct, &QAction::triggered, this, &MainWindow::serialSendPsk);

    m_printNameplateAct = new QAction(tr("Print Nameplate"));
    m_printNameplateAct->setStatusTip(tr("Print Nameplate"));
    connect(m_printNameplateAct, &QAction::triggered, this, &MainWindow::printNameplate);
    //connect(m_printNameplateAct, &QAction::triggered, m_testExplorer, &TestExplorer::printNameplate);

    m_programFWAct = new QAction(tr("Program Firmware"));
    m_programFWAct->setStatusTip(tr("Program Firmware"));
    connect(m_programFWAct, &QAction::triggered, this, &MainWindow::targetProgramFirmware);

    m_requestTokenAct = new QAction(tr("Request Token"));
    m_requestTokenAct->setStatusTip(tr("Request Token"));
    connect(m_requestTokenAct, &QAction::triggered, this, &MainWindow::requestToken);

    m_registerDeviceAct = new QAction(tr("Register Device"));
    m_registerDeviceAct->setStatusTip(tr("Registers the provided device with the contained metadata."));
    connect(m_registerDeviceAct, &QAction::triggered, this, &MainWindow::registerDevice);

    m_getPskAct = new QAction(tr("get PSK"));
    m_getPskAct->setStatusTip(tr("Gets the pre shared key for the device with the prodived id."));
    connect(m_getPskAct, &QAction::triggered, this, &MainWindow::getPsk);

    m_validateEncryptionAct = new QAction(tr("validate Encryption"));
    m_validateEncryptionAct->setStatusTip(tr("Validates if the client possess the correct pre shared key"));
    connect(m_validateEncryptionAct, &QAction::triggered, this, &MainWindow::validateEncryption);

    m_getLatestFwAct = new QAction(tr("download Firmware"));
    m_getLatestFwAct->setStatusTip(tr("Gets the latest stable version of the firmware with the provided name."));
    connect(m_getLatestFwAct, &QAction::triggered, this, &MainWindow::downloadLatestFirmware);

    m_runTestAct = new QAction(tr("run Test"));
    m_runTestAct->setStatusTip(tr(""));
    connect(m_runTestAct, &QAction::triggered, this, &MainWindow::runTest);

    m_skipStepAct = new QAction(tr("skip step"));
    m_skipStepAct->setStatusTip(tr(""));
    connect(m_skipStepAct, &QAction::triggered, this, &MainWindow::skip);
  }

  void MainWindow::createMenus()
  {
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_newAct);
//    m_fileMenu->addAction(m_openAct);
//    m_fileMenu->addAction(m_saveAct);
//    m_fileMenu->addAction(m_saveAsAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_printNameplateAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAct);

    m_targetMenu= menuBar()->addMenu(tr("&Target"));
    m_targetMenu->addAction(m_connectAct);
    m_targetMenu->addAction(m_programTSTAct);
    m_targetMenu->addAction(m_resetAct);
    m_targetMenu->addAction(m_programFWAct);

    m_serialMenu = menuBar()->addMenu(tr("&Serial"));
    m_serialMenu->addAction(m_serialConnectAct);
    m_serialMenu->addAction(m_serialSendSyncAct);
    m_serialMenu->addAction(m_serialSendPskAct);

    m_provisioningMenu = menuBar()->addMenu(tr("&Provisioning"));
    m_provisioningMenu->addAction(m_requestTokenAct);
    m_provisioningMenu->addSeparator();
    m_provisioningMenu->addAction(m_registerDeviceAct);
    m_provisioningMenu->addAction(m_getPskAct);
    m_provisioningMenu->addAction(m_validateEncryptionAct);
    m_provisioningMenu->addSeparator();
    m_provisioningMenu->addAction(m_getLatestFwAct);

    m_testMenu = menuBar()->addMenu(tr("&Test"));
    m_testMenu->addAction(m_runTestAct);
    m_testMenu->addAction(m_skipStepAct);

    m_helpMenu = menuBar()->addMenu(tr("&Help"));
    m_helpMenu->addAction(m_aboutAct);
  }

  void MainWindow::createToolBars()
  {
    m_fileToolBar = addToolBar(tr("File"));
    m_fileToolBar->setObjectName("fileToolBar");
    m_fileToolBar->addAction(m_newAct);
//    m_fileToolBar->addAction(m_openAct);
//    m_fileToolBar->addAction(m_saveAct);
    m_fileToolBar->addAction(m_printNameplateAct);

    m_targetToolBar = addToolBar(tr("Target"));
    m_targetToolBar->setObjectName("targetToolBar");
    m_targetToolBar->addAction(m_connectAct);
    m_targetToolBar->addAction(m_programTSTAct);
    m_targetToolBar->addAction(m_resetAct);
    m_targetToolBar->addAction(m_programFWAct);

    m_serialToolBar = addToolBar(tr("Serial"));
    m_serialToolBar->setObjectName("serialToolBar");
    m_serialToolBar->addAction(m_serialConnectAct);
    m_serialToolBar->addAction(m_serialSendSyncAct);
    m_serialToolBar->addAction(m_serialSendPskAct);

    m_provisioningToolBar = addToolBar(tr("Provisioning"));
    m_provisioningToolBar->setObjectName("provisioningToolBar");
    m_provisioningToolBar->addAction(m_requestTokenAct);
    m_provisioningToolBar->addAction(m_registerDeviceAct);
    m_provisioningToolBar->addAction(m_getPskAct);
    m_provisioningToolBar->addAction(m_validateEncryptionAct);
    m_provisioningToolBar->addAction(m_getLatestFwAct);

    m_testToolBar = addToolBar(tr("Test"));
    m_testToolBar->setObjectName("testToolBar");
    m_testToolBar->addAction(m_runTestAct);
    m_testToolBar->addAction(m_skipStepAct);

  }

  void MainWindow::createStatusBar()
  {
    statusBar()->showMessage(tr("Ready"));
  }

  void MainWindow::about()
  {
  }

  void MainWindow::closeEvent(QCloseEvent *event)
  {
    auto settings = m_settingsDialog->settings();
    settings.setValue(KEY_UI_GEOMETRY, saveGeometry());
    settings.setValue(KEY_UI_WINDOWSTATE, saveState());
    QMainWindow::closeEvent(event);
  }

}
