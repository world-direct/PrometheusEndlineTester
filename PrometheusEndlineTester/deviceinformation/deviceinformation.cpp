#include "deviceinformation.h"

namespace worlddirect {

  DeviceInformation::DeviceInformation(QObject *parent)
    : QObject(parent),
      m_uuid(),
      m_type(),
      m_hardwareVersion(),
      m_endpointName(),
      m_iccId(),
      m_psk()
  {
    clear();
  }

  DeviceInformation::~DeviceInformation()
  {

  }

  void DeviceInformation::clear()
  {
    setUuid( QUuid::createUuid());
    setType(QString());
    setIccId(QString());
    setEndpointName(QString());
    setHardwareVersion(QString());
    setPSK(QVector<quint8>());
  }

  bool DeviceInformation::valid() const
  {
      if(complete() == false){
          return false;
      }

    if(iccId().isEmpty()){
        return false;
      }

    return  true;
  }

  bool DeviceInformation::complete() const
  {
    if(type().isEmpty()){
        return false;
      }

    if(endpointName().isEmpty()){
        return false;
      }

    if(hardwareVersion().isEmpty()){
        return false;
      }

    return  true;
  }

  QUuid DeviceInformation::uuid() const
  {
    return m_uuid;
  }

  QString DeviceInformation::testRun() const
  {
    return uuid().toString(QUuid::WithoutBraces);
  }

  QString DeviceInformation::type() const
  {
    return m_type;
  }

  void DeviceInformation::setType(const QString &ty)
  {
    if(type().compare(ty) != 0){
        m_type = ty;
        emit typeChanged(m_type);
      }
    if(complete()){
        emit baseInformationComplete();
    }
  }

  QString DeviceInformation::hardwareVersion() const
  {
    return m_hardwareVersion;
  }

  void DeviceInformation::setHardwareVersion(const QString &hwVersion)
  {
    if(hardwareVersion().compare(hwVersion) != 0){
        m_hardwareVersion = hwVersion;
        emit hardwareVersionChanged(m_hardwareVersion);
      }
    if(complete()){
        emit baseInformationComplete();
    }
  }

  QString DeviceInformation::endpointName() const
  {
    return m_endpointName;
  }

  void DeviceInformation::setEndpointName(const QString &epName)
  {
    if(endpointName().compare(epName) != 0){
        m_endpointName = epName;
        emit endpointNameChanged(m_endpointName);
      }
    if(complete()){
        emit baseInformationComplete();
    }
  }

  QString DeviceInformation::iccId() const
  {
    return m_iccId;
  }

  void DeviceInformation::setUuid(const QUuid &id)
  {
    if(uuid() != id){
        m_uuid = id;
        emit uuidChanged(m_uuid);
        emit testRunChanged(testRun());
      }
  }
  
  QVector<quint8> DeviceInformation::psk() const
  {
    return m_psk;
  }
  
  void DeviceInformation::setIccId(const QString &icc)
  {
    if(iccId().compare(icc) != 0){
        m_iccId = icc;
        emit iccIdChanged(m_iccId);
      }

    if(valid()){
        emit deviceInformationValid();
    }
  }

  void DeviceInformation::setPSK(const QVector<quint8> &key)
  {
    m_psk = key;
  }

} // namespace worlddirect
