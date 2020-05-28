#include "stlinkdeviceflasher.h"
#include <QIODevice>
#include <QSettings>

#include "PrometheusEndlineTester_global.h"

namespace worlddirect {

  StLinkDeviceFlasher::StLinkDeviceFlasher(QObject *parent)
    :QProcess(parent),
      m_mode(Mode::NONE)
  {
    connect(this, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finishedSlot(int,QProcess::ExitStatus)));
    connect(this, &StLinkDeviceFlasher::readyReadStandardOutput, this, &StLinkDeviceFlasher::readProcessData);
    connect(this, &StLinkDeviceFlasher::readyReadStandardError, this, &StLinkDeviceFlasher::readProcessData);
  }

  void StLinkDeviceFlasher::connectToTarget()
  {
    if (state() != QProcess::NotRunning){
        return;
      }

    QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);
    auto pathToCmd = settings.value(KEY_FLASHER_PATHTOCMD).toString();
    auto connectArgs = settings.value(KEY_FLASHER_CONNECTARGS).toString();

    m_mode = Mode::CONNECT;
    this->start(pathToCmd, {connectArgs}, QIODevice::ReadOnly);

  }

  void StLinkDeviceFlasher::programTarget()
  {
    if (state() != QProcess::NotRunning){
        return;
      }

    QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);
    auto pathToCmd = settings.value(KEY_FLASHER_PATHTOCMD).toString();
    auto writeArgs = settings.value(KEY_FLASHER_WRITEARGS).toString();
    auto pathToFirmware = settings.value(KEY_FLASHER_PATHTOFW).toString();

    m_mode = Mode::PROGRAM;
    this->start(pathToCmd, {writeArgs, pathToFirmware}, QIODevice::ReadOnly);
  }

  void StLinkDeviceFlasher::resetTarget()
  {
    if (state() != QProcess::NotRunning){
        return;
      }

    QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);
    auto pathToCmd = settings.value(KEY_FLASHER_PATHTOCMD).toString();
    auto resetArgs = settings.value(KEY_FLASHER_RESETARGS).toString();

    m_mode = Mode::RESET;
    this->start(pathToCmd, {resetArgs}, QIODevice::ReadOnly);

  }

  void StLinkDeviceFlasher::readProcessData()
  {
    //auto dt = readAll();
    while (canReadLine()) {
        auto dt = readLine().trimmed();
        emit newLineReceived(dt);
      }
    //emit printData(dt);
  }

  void StLinkDeviceFlasher::errorOccurredSlot(QProcess::ProcessError error)
  {
    Q_UNUSED(error);
    emit exitFailure();
    m_mode = Mode::NONE;
  }

  void StLinkDeviceFlasher::finishedSlot(int exitCode, QProcess::ExitStatus exitStatus)
  {
    if(exitCode != 0 || exitStatus != QProcess::NormalExit)
      {
        emit exitFailure();
      }

    switch (m_mode) {
      case Mode::NONE:{break;}
      case Mode::CONNECT:{
          emit exitSuccess();
          emit targetConnected();
          break;
        }
      case Mode::PROGRAM:{
          emit exitSuccess();
          emit targetProgrammed();
          break;
        }
      case Mode::RESET:{
          emit exitSuccess();
          emit targetReset();
          break;
        }
      default:{
          emit exitFailure();
          break;
        }
      }
    m_mode = Mode::NONE;
  }



} // namespace worlddirect
