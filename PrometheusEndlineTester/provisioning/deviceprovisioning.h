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

    void successMessage(const QString& msg);
    void errorMessage(const QString& msg);

    void pskReceived(const QString& key);

  public slots:
    void getToken();

    void downloadLatestFirmware();

    void createDevice(const QString& epName, const QString& type, const QString& hwVer, const QString& iccId);

    // void getPsk(const QString& epName);

    // void testPsk(const QString& epName);
    void getPsk(const QString& epName);
    void validatePsk(const QString& epName, const QString &iccId, const QString &psk);


  private slots:
    void setToken(const QString &s);


  private:
    QString m_token;

  };

} // namespace worlddirect

#endif // WORLDDIRECT_DEVICEPROVISIONING_H
