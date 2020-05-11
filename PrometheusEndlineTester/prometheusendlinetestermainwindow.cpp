#include "prometheusendlinetestermainwindow.h"
#include "./ui_prometheusendlinetestermainwindow.h"

PrometheusEndlineTesterMainWindow::PrometheusEndlineTesterMainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::PrometheusEndlineTesterMainWindow)
{
  ui->setupUi(this);
}

PrometheusEndlineTesterMainWindow::~PrometheusEndlineTesterMainWindow()
{
  delete ui;
}

