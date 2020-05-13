#include <QApplication>
#include <QSettings>
#include <QSerialPort>
#include <iostream>
#include <QUuid>

#include "PrometheusEndlineTester_global.h"
#include "stlinkdeviceflasher.h"
#include "mbedhosttestrunner.h"
#include "ui/mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("World-Direct");
  QCoreApplication::setApplicationVersion(PROJECT_VER);
  QCoreApplication::setOrganizationDomain("https://www.world-direct.at/");
  QCoreApplication::setApplicationName(PROJECT_NAME);

  QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);

  auto pathToCmd = settings.value(KEY_FLASHER_PATHTOCMD).toString();
  auto writeArgs = settings.value(KEY_FLASHER_WRITEARGS).toString();
  auto eraseArgs = settings.value(KEY_FLASHER_ERASEARGS).toString();
  auto resetArgs = settings.value(KEY_FLASHER_RESETARGS).toString();
  auto pathToFirmware = settings.value(KEY_FLASHER_PATHTOFW).toString();

  auto port = settings.value(KEY_SERIAL_PORT).toString();
  auto baudRate = settings.value(KEY_SERIAL_BAUD).toInt();
  auto dataBits = settings.value(KEY_SERIAL_DATA).toInt();
  auto parity = settings.value(KEY_SERIAL_PARITY).toInt();
  auto stopBits = settings.value(KEY_SERIAL_STOP).toInt();
  auto flowControl = settings.value(KEY_SERIAL_FLOWCTRL).toInt();

  //  worlddirect::StLinkDeviceFlasher stflash(pathToCmd.toStdString());
  //  stflash.erase(eraseArgs.toStdString());
  //  stflash.write(writeArgs.toStdString(), pathToFirmware.toStdString());
  //  stflash.reset(resetArgs.toStdString());

  // auto uuid = QUuid::createUuid();
  // worlddirect::MbedHostTestRunner htrun;
  // htrun.connect(port, baudRate, dataBits, parity, stopBits, flowControl);
  // htrun.sendSync(uuid);
  // htrun.disconnect();

  worlddirect::MainWindow w;
  w.readSettings();
  w.show();
  a.exec();

  settings.setValue(KEY_FLASHER_PATHTOCMD, pathToCmd);
  settings.setValue(KEY_FLASHER_WRITEARGS, writeArgs);
  settings.setValue(KEY_FLASHER_ERASEARGS, eraseArgs);
  settings.setValue(KEY_FLASHER_RESETARGS, resetArgs);
  settings.setValue(KEY_FLASHER_PATHTOFW, pathToFirmware);

  settings.setValue(KEY_SERIAL_PORT, QVariant::fromValue(port));
  settings.setValue(KEY_SERIAL_BAUD, QVariant::fromValue(baudRate));
  settings.setValue(KEY_SERIAL_DATA, QVariant::fromValue(dataBits));
  settings.setValue(KEY_SERIAL_PARITY, QVariant::fromValue(parity));
  settings.setValue(KEY_SERIAL_STOP, QVariant::fromValue(stopBits));
  settings.setValue(KEY_SERIAL_FLOWCTRL, QVariant::fromValue(flowControl));

  return EXIT_SUCCESS;
}
