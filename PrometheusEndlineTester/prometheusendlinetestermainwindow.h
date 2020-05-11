#ifndef PROMETHEUSENDLINETESTERMAINWINDOW_H
#define PROMETHEUSENDLINETESTERMAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class PrometheusEndlineTesterMainWindow; }
QT_END_NAMESPACE

class PrometheusEndlineTesterMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  PrometheusEndlineTesterMainWindow(QWidget *parent = nullptr);
  ~PrometheusEndlineTesterMainWindow();

private:
  Ui::PrometheusEndlineTesterMainWindow *ui;
};
#endif // PROMETHEUSENDLINETESTERMAINWINDOW_H
