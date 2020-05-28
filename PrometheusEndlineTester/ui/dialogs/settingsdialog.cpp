#include "settingsdialog.h"

#include "PrometheusEndlineTester_global.h"

namespace worlddirect {


  SettingsDialog::SettingsDialog(QWidget *parent)
      :QDialog(parent)
  {

  }

  QSettings SettingsDialog::settings() const
  {
      return QSettings(SETT_FILE_NAME, QSettings::IniFormat);
  }

} // namespace worlddirect
