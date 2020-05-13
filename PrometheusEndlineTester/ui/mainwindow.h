#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>

namespace worlddirect {

    class MainWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = nullptr);
        MainWindow&  operator= ( const  MainWindow& ) = delete;
        MainWindow&  operator= ( const  MainWindow&& ) = delete;

        MainWindow ( const  MainWindow& ) = delete;
        MainWindow ( const  MainWindow&& ) = delete;

        virtual ~MainWindow() = default;

        void readSettings();

    private:
        void createActions();
        void createMenus();
        void createToolBars();
        void createStatusBar();

    protected:
        virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

    private:
        QWidget* m_centralWidget;
    };
}

#endif // MAINWINDOW_H
