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
      m_programAct(Q_NULLPTR),
      m_resetAct(Q_NULLPTR),
      m_serialConnectAct(Q_NULLPTR),
      m_serialSendSyncAct(Q_NULLPTR),
      m_printNameplateAct(Q_NULLPTR),
      m_fileMenu(Q_NULLPTR),
      m_targetMenu(Q_NULLPTR),
      m_serialMenu(Q_NULLPTR),
      m_helpMenu(Q_NULLPTR),
      m_fileToolBar(Q_NULLPTR),
      m_targetToolBar(Q_NULLPTR),
      m_serialToolBar(Q_NULLPTR)
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

  void MainWindow::setNewTargetState()
  {

  }

  void MainWindow::setTargetNotConnectedState()
  {

  }

  void MainWindow::setTargetConnectedState()
  {

  }

  void MainWindow::setTargetNotProgrammedState()
  {

  }

  void MainWindow::setTargetProgrammedState()
  {

  }

  void MainWindow::setTargetNotResetState()
  {

  }

  void MainWindow::setTargetResetState()
  {

  }

  void MainWindow::setSerialNotConnectedState()
  {

  }

  void MainWindow::setSerialConnectedState()
  {

  }

  void MainWindow::setSerialNotSyncedState()
  {

  }

  void MainWindow::setSerialSyncedState()
  {

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


  void MainWindow::createActions()
  {
    m_newAct = new QAction(tr("&New"), this);
    m_newAct->setShortcuts(QKeySequence::New);
    m_newAct->setStatusTip(tr("Create a new file"));
    connect(m_newAct, &QAction::triggered, this, &MainWindow::newFile);
    connect(m_newAct, &QAction::triggered, this, &MainWindow::targetNew);

    m_openAct = new QAction(tr("&Open..."), this);
    m_openAct->setShortcuts(QKeySequence::Open);
    m_openAct->setStatusTip(tr("Open an existing file"));
    connect(m_openAct, &QAction::triggered, this, &MainWindow::open);

    m_saveAct = new QAction(tr("&Save"), this);
    m_saveAct->setShortcuts(QKeySequence::Save);
    m_saveAct->setStatusTip(tr("Save the document to disk"));
    connect(m_saveAct, &QAction::triggered, this, &MainWindow::save);

    m_saveAsAct = new QAction(tr("Save &As..."), this);
    m_saveAsAct->setShortcuts(QKeySequence::SaveAs);
    m_saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(m_saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);

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

    m_programAct = new QAction(tr("Program & Verify"), this);
    m_programAct->setStatusTip(tr("Program & Verify"));
    connect(m_programAct, &QAction::triggered, this, &MainWindow::targetProgram);

    m_resetAct = new QAction(tr("Reset"));
    m_resetAct->setStatusTip(tr("reset taget"));
    connect(m_resetAct, &QAction::triggered, this, &MainWindow::targetReset);

    m_serialConnectAct = new QAction(tr("Connect Serial"));
    m_serialConnectAct->setStatusTip(tr("Connect to the target"));
    connect(m_serialConnectAct, &QAction::triggered, this, &MainWindow::serialConnect);

    m_serialSendSyncAct = new QAction(tr("Send Sync"), this);
    m_serialSendSyncAct->setStatusTip(tr("send sync message"));
    connect(m_serialSendSyncAct, &QAction::triggered, this, &MainWindow::serialSendSync);

    m_printNameplateAct = new QAction(tr("Print Nameplate"));
    m_printNameplateAct->setStatusTip(tr("Print Nameplate"));
    connect(m_printNameplateAct, &QAction::triggered, this, &MainWindow::printNameplate);

  }

  void MainWindow::createMenus()
  {
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_newAct);
    m_fileMenu->addAction(m_openAct);
    m_fileMenu->addAction(m_saveAct);
    m_fileMenu->addAction(m_saveAsAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_printNameplateAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAct);

    m_targetMenu= menuBar()->addMenu(tr("&Target"));
    m_targetMenu->addAction(m_connectAct);
    m_targetMenu->addAction(m_programAct);
    m_targetMenu->addAction(m_resetAct);

    m_serialMenu = menuBar()->addMenu(tr("&Serial"));
    m_serialMenu->addAction(m_serialConnectAct);
    m_serialMenu->addAction(m_serialSendSyncAct);

    m_helpMenu = menuBar()->addMenu(tr("&Help"));
    m_helpMenu->addAction(m_aboutAct);
  }

  void MainWindow::createToolBars()
  {
    m_fileToolBar = addToolBar(tr("File"));
    m_fileToolBar->setObjectName("fileToolBar");
    m_fileToolBar->addAction(m_newAct);
    m_fileToolBar->addAction(m_openAct);
    m_fileToolBar->addAction(m_saveAct);
    m_fileToolBar->addAction(m_printNameplateAct);

    m_targetToolBar = addToolBar(tr("Target"));
    m_targetToolBar->setObjectName("targetToolBar");
    m_targetToolBar->addAction(m_connectAct);
    m_targetToolBar->addAction(m_programAct);
    m_targetToolBar->addAction(m_resetAct);

    m_serialToolBar = addToolBar(tr("Serial"));
    m_serialToolBar->setObjectName("serialToolBar");
    m_serialToolBar->addAction(m_serialConnectAct);
    m_serialToolBar->addAction(m_serialSendSyncAct);
  }

  void MainWindow::createStatusBar()
  {
    statusBar()->showMessage(tr("Ready"));
  }

  void MainWindow::newFile()
  {

  }

  void MainWindow::open()
  {

  }

  bool MainWindow::save()
  {
    return false;
  }

  bool MainWindow::saveAs()
  {
    return false;
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
