#ifndef WORLDDIRECT_SETTINGS_H
#define WORLDDIRECT_SETTINGS_H

#include <QObject>

namespace worlddirect {

    class Settings : public QObject
    {
        Q_OBJECT
    public:
        explicit Settings(QObject *parent = nullptr);
        Settings ( const  Settings& ) = delete;
        Settings ( const  Settings&& ) = delete;

        Settings&  operator= ( const  Settings& ) = delete;
        Settings&  operator= ( const  Settings&& ) = delete;

        virtual ~Settings() = default;

    };

} // namespace worlddirect

#endif // WORLDDIRECT_SETTINGS_H
