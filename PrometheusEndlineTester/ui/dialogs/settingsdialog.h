#ifndef WORLDDIRECT_SETTINGSDIALOG_H
#define WORLDDIRECT_SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

namespace worlddirect {

    class SettingsDialog: public QDialog
    {
        Q_OBJECT
    public:
        explicit SettingsDialog(QWidget *parent = Q_NULLPTR);
        SettingsDialog&  operator= ( const  SettingsDialog& ) = delete;
        SettingsDialog&  operator= ( const  SettingsDialog&& ) = delete;

        SettingsDialog ( const  SettingsDialog& ) = delete;
        SettingsDialog ( const  SettingsDialog&& ) = delete;

        virtual ~SettingsDialog() = default;

        QSettings settings()const;
    };

} // namespace worlddirect

#endif // WORLDDIRECT_SETTINGSDIALOG_H
