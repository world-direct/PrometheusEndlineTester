#include "stlinkdeviceflasher.h"
#include <QIODevice>
#include <QSettings>

#include "PrometheusEndlineTester_global.h"

namespace worlddirect {

  StLinkDeviceFlasher::StLinkDeviceFlasher(QObject *parent)
    :QProcess(parent),
      m_mode(Mode::NONE),
      m_testRun()
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
    auto connectArgs = settings.value(KEY_FLASHER_CONNECTARGS).toString();

    m_mode = Mode::CONNECT;
    this->start(pathToCmd(), {connectArgs}, QIODevice::ReadOnly);

  }

  void StLinkDeviceFlasher::programTargetTest()
  {
    programTarget(BinaryType::TEST);
  }

  void StLinkDeviceFlasher::resetTarget()
  {
    if (state() != QProcess::NotRunning){
        return;
      }

    QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);
    auto resetArgs = settings.value(KEY_FLASHER_RESETARGS).toString();

    m_mode = Mode::RESET;
    this->start(pathToCmd(), {resetArgs}, QIODevice::ReadOnly);

  }

  void StLinkDeviceFlasher::programTargetFirmware()
  {
    programTarget(BinaryType::FIRMWARE);
  }

  void StLinkDeviceFlasher::programTarget(StLinkDeviceFlasher::BinaryType type)
  {
    if (state() != QProcess::NotRunning){
        return;
      }

    QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);
    auto writeArgs = settings.value(KEY_FLASHER_WRITEARGS).toString();

    auto binaryKey =KEY_FLASHER_PATHTOTEST ;
    switch (type) {
      case BinaryType::NONE: {return;}
      case BinaryType::TEST: {
          binaryKey = KEY_FLASHER_PATHTOTEST;
          break;
        }
      case BinaryType::FIRMWARE: {
          binaryKey = KEY_FLASHER_PATHTOFW;
          break;
        }
      default:{return;}
      }
    auto pathToFirmware = settings.value(binaryKey).toString();

    m_mode = Mode::PROGRAM;
    this->start(pathToCmd(), {writeArgs, pathToFirmware}, QIODevice::ReadOnly);

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
    auto prerror = tr(" Process Error: ");
    switch (error) {
      case Crashed:{
          emit errorMessage(mode2String(m_mode) + prerror + tr("Crashed"));
          break;}
      case Timedout:{
          emit errorMessage(mode2String(m_mode) + prerror + tr("Timed out"));
          break;
        }
      case ReadError:{
          emit errorMessage(mode2String(m_mode) + prerror + tr("Read Error"));
          break;
        }
      case WriteError:{
          emit errorMessage(mode2String(m_mode) + prerror  + tr("Write Error"));
          break;
        }
      default:{
          emit errorMessage(mode2String(m_mode) + prerror + tr("UNKOWN"));
          break;
        }
      }

    m_mode = Mode::NONE;
  }

  void StLinkDeviceFlasher::finishedSlot(int exitCode, QProcess::ExitStatus exitStatus)
  {
    if(exitCode != 0)
      {
        auto errorMsg = QString(tr(" returned with error: %1")).arg( exitCode );
        emit errorMessage(mode2String(m_mode) + errorMsg);
        return;
      }

    if(exitStatus != QProcess::NormalExit)
      {
        emit errorMessage(mode2String(m_mode) +" "+ tr("Crashed") );
        return;
      }

    switch (m_mode) {
      case Mode::NONE:{break;}
      case Mode::CONNECT:{
          emit(successMessage(mode2String(m_mode)+ tr(" successfully finished")));
          emit targetConnected();
          break;
        }
      case Mode::PROGRAM:{
          emit(successMessage(mode2String(m_mode)+ tr(" successfully finished")));
          emit targetProgrammed();
          break;
        }
      case Mode::RESET:{
          emit(successMessage(mode2String(m_mode)+ tr(" successfully finished")));
          emit targetReset();
          break;
        }
      default:{
          emit errorMessage(mode2String(m_mode) + tr(" error: Unkown operation"));
          break;
        }
      }
    m_mode = Mode::NONE;
  }

  QString StLinkDeviceFlasher::mode2String(StLinkDeviceFlasher::Mode md)
  {
    switch (md) {
      case Mode::NONE:{return tr("ST-LINK NONE");}
      case Mode::RESET:{return tr("ST-LINK Reset");}
      case Mode::CONNECT:{return tr("ST-LINK Connect");}
      case Mode::PROGRAM:{return tr("ST-LINK Program");}
      }
    return tr("ST-LINK");
  }

  QString StLinkDeviceFlasher::pathToCmd() const
  {
    QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);
    auto basePath = settings.value(KEY_FLASHER_BASEPATH).toString();
    auto cmd = settings.value(KEY_FLASHER_PATHTOCMD).toString();

    return basePath + cmd;
  }

} // namespace worlddirect
