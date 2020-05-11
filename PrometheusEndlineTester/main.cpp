#include "prometheusendlinetestermainwindow.h"

#include <QApplication>
#include "prometheusendlinetesterlib.h"

int main(int argc, char *argv[])
{
  PrometheusEndlineTesterLib tmp;

  QApplication a(argc, argv);
  PrometheusEndlineTesterMainWindow w;
  w.show();
  return a.exec();
}
