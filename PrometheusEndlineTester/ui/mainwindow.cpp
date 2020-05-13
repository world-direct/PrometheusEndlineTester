#include "mainwindow.h"

#include <QGridLayout>
#include <QSettings>

#include "PrometheusEndlineTester_global.h"

namespace worlddirect{

  MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_centralWidget(new QWidget(this))
  {
    setContentsMargins(0,0,0,0);
    setWindowTitle(PROJECT_NAME);

    auto m_layout = new QGridLayout();
    m_layout->setMargin(0);
    m_layout->setHorizontalSpacing(0);
    m_layout->setVerticalSpacing(0);
    m_centralWidget->setLayout(m_layout);

    setCentralWidget(m_centralWidget);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
  }

  void MainWindow::readSettings()
  {
    QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);
    restoreGeometry(settings.value(KEY_UI_GEOMETRY).toByteArray());
    restoreState(settings.value(KEY_UI_WINDOWSTATE).toByteArray());
  }

  void MainWindow::createActions()
  {

  }

  void MainWindow::createMenus()
  {

  }

  void MainWindow::createToolBars()
  {

  }

  void MainWindow::createStatusBar()
  {

  }

  void MainWindow::closeEvent(QCloseEvent *event)
  {
    QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);
    settings.setValue(KEY_UI_GEOMETRY, saveGeometry());
    settings.setValue(KEY_UI_WINDOWSTATE, saveState());
    QMainWindow::closeEvent(event);
  }

}
