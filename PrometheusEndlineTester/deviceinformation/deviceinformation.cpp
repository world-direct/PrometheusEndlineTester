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
    setPSK(QString());

//    setEndpointName("CO139X92IMH9154P");
//    setIccId("123345552342343");
//    setHardwareVersion("2.1.0.12345");
//    setType("Wd IoT Coreboard");
  }

  bool DeviceInformation::valid() const
  {
    if(type().isEmpty()){
        return false;
      }

    if(endpointName().isEmpty()){
        return false;
      }

    if(iccId().isEmpty()){
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
  
  QString DeviceInformation::psk() const
  {
    return m_psk;
  }
  
  void DeviceInformation::setIccId(const QString &icc)
  {
    if(iccId().compare(icc) != 0){
        m_iccId = icc;
        emit iccIdChanged(m_iccId);
      }
  }

  void DeviceInformation::setPSK(const QString &key)
  {
    m_psk = key;
  }

} // namespace worlddirect
