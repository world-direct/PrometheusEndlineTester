//#include "prometheusendlinetestermainwindow.h"
#include <QApplication>
#include <QSettings>
#include <iostream>

#include "PrometheusEndlineTester_global.h"
#include "stlinkdeviceflasher.h"


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

  worlddirect::StLinkDeviceFlasher stflash(pathToCmd.toStdString());
  stflash.erase(eraseArgs.toStdString());
  stflash.write(writeArgs.toStdString(), pathToFirmware.toStdString());
  stflash.reset(resetArgs.toStdString());

  settings.setValue(KEY_FLASHER_PATHTOCMD, pathToCmd);
  settings.setValue(KEY_FLASHER_WRITEARGS, writeArgs);
  settings.setValue(KEY_FLASHER_ERASEARGS, eraseArgs);
  settings.setValue(KEY_FLASHER_RESETARGS, resetArgs);
  settings.setValue(KEY_FLASHER_PATHTOFW, pathToFirmware);

  //PrometheusEndlineTesterMainWindow w;
  //w.show();
  //return a.exec();
  return EXIT_SUCCESS;
}
