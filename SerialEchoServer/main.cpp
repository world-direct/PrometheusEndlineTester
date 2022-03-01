#include <QCoreApplication>
#include <QSettings>
#include <QSerialPort>
#include <iostream>
#include <QVector>
#include <QTimer>

#include "SerialEchoServer_global.h"
#include "clientthread.h"

int main(int argc, char *argv[])
{
  qRegisterMetaType<QVector<quint8> >("QVector<quint8>");
  QCoreApplication a(argc, argv);
  QCoreApplication::setOrganizationName("World-Direct eBusiness solutions GmbH");
  QCoreApplication::setApplicationVersion(PROJECT_VER);
  QCoreApplication::setOrganizationDomain("https://www.world-direct.at/");
  QCoreApplication::setApplicationName("Serial Echo Server");


  ClientThread serial_client;

  QTimer::singleShot(1000, &serial_client, &ClientThread::startClient);
  auto res = a.exec();

  return res;

}
