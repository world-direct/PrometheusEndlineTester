#include <QApplication>
#include <QSettings>
#include <QSerialPort>
#include <iostream>
#include <QUuid>
#include <QFile>

#include "PrometheusEndlineTester_global.h"
#include <statemachine/statemachine.h>

#include "identityclient.h"
#include "deviceprovisioningapiclient.h"
#include <iostream>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("World-Direct eBusiness solutions GmbH");
  QCoreApplication::setApplicationVersion(PROJECT_VER);
  QCoreApplication::setOrganizationDomain("https://www.world-direct.at/");
  QCoreApplication::setApplicationName(PROJECT_NAME);

  // load stylesheet if exists
  QFile file(QSS_FILE_NAME);
  if(file.open(QFile::ReadOnly))
    {
        QString styleSheet = QString::fromLatin1(file.readAll());
        //ui.styleTextEdit->setPlainText(styleSheet);
        qApp->setStyleSheet(styleSheet);
    }

  worlddirect::StateMachine sm(&a);
  sm.start();
  auto res = a.exec();
  sm.stop();



  return res;
}
