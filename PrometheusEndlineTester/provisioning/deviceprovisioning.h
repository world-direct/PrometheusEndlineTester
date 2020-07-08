#ifndef WORLDDIRECT_DEVICEPROVISIONING_H
#define WORLDDIRECT_DEVICEPROVISIONING_H

#include <QObject>

namespace worlddirect {

    class DeviceProvisioning : public QObject
    {
        Q_OBJECT
    public:
        explicit DeviceProvisioning(QObject *parent = nullptr);
        DeviceProvisioning&  operator= ( const  DeviceProvisioning& ) = delete;
        DeviceProvisioning&  operator= ( const  DeviceProvisioning&& ) = delete;

        DeviceProvisioning ( const  DeviceProvisioning& ) = delete;
        DeviceProvisioning ( const  DeviceProvisioning&& ) = delete;

        virtual ~DeviceProvisioning()= default;

    signals:
        void printData(const QByteArray &dt);
        void resultReady(const QString &s);

    public slots:
        void getToken();

        void downloadLatestFirmware();

        // void createDevice(const QString& epName, const QString& type, const QString& hwVer, const QString& iccId);

        // void getPsk(const QString& epName);

        // void testPsk(const QString& epName);

    private slots:
        void setToken(const QString &s);


    private:
        QString m_token;

    };

} // namespace worlddirect

#endif // WORLDDIRECT_DEVICEPROVISIONING_H
