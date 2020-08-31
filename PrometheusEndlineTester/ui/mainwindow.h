#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>


QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QToolBar;
class QStateMachine;
QT_END_NAMESPACE


namespace worlddirect {

  class SettingsDialog;
  class Console;
  class TestExplorer;
  class AbstractTest;

  class MainWindow : public QMainWindow
  {
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    MainWindow&  operator= ( const  MainWindow& ) = delete;
    MainWindow&  operator= ( const  MainWindow&& ) = delete;

    MainWindow ( const  MainWindow& ) = delete;
    MainWindow ( const  MainWindow&& ) = delete;

    virtual ~MainWindow() = default;

  signals:
    void printData(const QByteArray &dt);
    void clearScreen();

    void targetConnect();
    void targetProgramTest();
    void targetReset();
    void serialConnect();
    void serialSendSync();
    void serialSendPsk();
    void printNameplate();
    void targetProgramFirmware();
    void requestToken();
    void registerDevice();
    void getPsk();
    void validateEncryption();
    void downloadLatestFirmware();
    void runTest();
    void skip();
    void retry();
    void newTarget();

  public slots:
    void readSettings();

    void newHostTestRun(const QString& hostTestRun);
    void newHostTest(const QString& hostTestRun, const QString& hostTest);
    void newTestCase(const QString& hostTestRun, const QString& hostTest, const QString& hostTestCase);
    void startTestCase(const QString& hostTestRun, const QString& hostTest, const QString& hostTestCase);
    void passedTestCase(const QString& hostTestRun, const QString& hostTest, const QString& hostTestCase);
    void failedTestCase(const QString& hostTestRun, const QString& hostTest, const QString& hostTestCase);
    void passedHostTest(const QString& hostTestRun, const QString& hostTest);
    void failedHostTest(const QString& hostTestRun, const QString& hostTest);
    void passedHostTestRun(const QString& hostTestRun);
    void failedHostTestRun(const QString& hostTestRun);

    void typeReceived(const QString& type);
    void hardwareVersionReceived(const QString& hwVer);
    void endpointNameReceived(const QString& epName);
    void iccIdReceived(const QString& iccId);

    void nameplateValid();

  private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

  private slots:
    //void newFile();
    //void open();
    //bool save();

    //bool saveAs();
    void about();

  protected:
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

  private:
    QWidget* m_centralWidget;
    Console *m_console;
    TestExplorer* m_testExplorer;
    SettingsDialog* m_settingsDialog;

    QAction* m_newAct;
    QAction* m_openAct;
    QAction* m_saveAct;
    QAction* m_saveAsAct;
    QAction* m_exitAct;
    QAction* m_aboutAct;
    QAction* m_connectAct;
    QAction* m_programTSTAct;
    QAction* m_resetAct;
    QAction* m_serialConnectAct;
    QAction* m_serialSendSyncAct;
    QAction* m_serialSendPskAct;
    QAction* m_printNameplateAct;
    QAction* m_programFWAct;
    QAction* m_requestTokenAct;
    QAction* m_registerDeviceAct;
    QAction* m_getPskAct;
    QAction* m_validateEncryptionAct;
    QAction* m_getLatestFwAct;
    QAction* m_runTestAct;
    QAction* m_skipStepAct;
    QAction* m_retryStepAct;

    QMenu* m_fileMenu;
    QMenu* m_targetMenu;
    QMenu* m_serialMenu;
    QMenu* m_provisioningMenu;
    QMenu* m_testMenu;
    QMenu* m_helpMenu;

    QToolBar* m_fileToolBar;
    QToolBar* m_targetToolBar;
    QToolBar* m_serialToolBar;
    QToolBar* m_provisioningToolBar;
    QToolBar* m_testToolBar;

  };
}

#endif // MAINWINDOW_H
