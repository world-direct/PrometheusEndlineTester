#ifndef WORLDDIRECT_DEVICEINFORMATION_H
#define WORLDDIRECT_DEVICEINFORMATION_H

#include <QObject>
#include <QUuid>
#include <QString>
#include <QVector>

namespace worlddirect {

  class DeviceInformation : public QObject
  {
    Q_OBJECT
  public:
    explicit DeviceInformation(QObject *parent = nullptr);
    DeviceInformation&  operator= ( const  DeviceInformation& ) = delete;
    DeviceInformation&  operator= ( const  DeviceInformation&& ) = delete;

    DeviceInformation ( const  DeviceInformation& ) = delete;
    DeviceInformation ( const  DeviceInformation&& ) = delete;

    virtual ~DeviceInformation();

  signals:
    void uuidChanged(const QUuid& uuid);
    void testRunChanged(const QString& tstRun);
    void typeChanged(const QString& ty);
    void hardwareVersionChanged(const QString &hwVersion);
    void endpointNameChanged(const QString& epName);
    void iccIdChanged(const QString icc);
    void baseInformationComplete();
    void deviceInformationValid();

  public slots:
    void setType(const QString &ty);
    void setHardwareVersion(const QString &hwVersion);
    void setEndpointName(const QString &epName);
    void setIccId(const QString &icc);
    void setPSK(const QVector<quint8> &key);

  public:
    void clear();
    bool valid()const;
    bool complete() const;

    QUuid uuid() const;
    QString testRun() const;
    QString type() const;
    QString hardwareVersion() const;
    QString endpointName() const;
    QString iccId() const;
    QVector<quint8> psk() const;

  private:
    void setUuid(const QUuid& id);

  private:
    QUuid  m_uuid;
    QString m_type;
    QString m_hardwareVersion;
    QString m_endpointName;
    QString m_iccId;
    QVector<quint8> m_psk;

  };

} // namespace worlddirect

#endif // WORLDDIRECT_DEVICEINFORMATION_H
