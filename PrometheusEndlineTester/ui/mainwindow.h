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
    void targetNew();
    void targetConnect();
    void targetProgram();
    void targetReset();
    void serialConnect();
    void serialSendSync();
    void printNameplate();

  public slots:
    void readSettings();

    void setNewTargetState();
    void setTargetNotConnectedState();
    void setTargetConnectedState();
    void setTargetNotProgrammedState();
    void setTargetProgrammedState();
    void setTargetNotResetState();
    void setTargetResetState();
    void setSerialNotConnectedState();
    void setSerialConnectedState();
    void setSerialNotSyncedState();
    void setSerialSyncedState();

    void newHostTestRun(const QString& hostTestRun);
    void newHostTest(const QString& hostTestRun, const QString& hostTest);
    void newTestCase(const QString& hostTestRun, const QString& hostTest, const QString& hostTestCase);

  private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

  private slots:
    void newFile();
    void open();
    bool save();

    bool saveAs();
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
    QAction* m_programAct;
    QAction* m_resetAct;
    QAction* m_serialConnectAct;
    QAction* m_serialSendSyncAct;
    QAction* m_printNameplateAct;

    QMenu* m_fileMenu;
    QMenu* m_targetMenu;
    QMenu* m_serialMenu;
    QMenu* m_helpMenu;

    QToolBar* m_fileToolBar;
    QToolBar* m_targetToolBar;
    QToolBar* m_serialToolBar;

  };
}

#endif // MAINWINDOW_H
