#include "deviceprovisioning.h"

#include <QSettings>
#include <QThread>
#include <QDebug>

#include <identityclient.h>
#include <deviceprovisioningapiclient.h>
#include <mbedcrypto.h>

#include "PrometheusEndlineTester_global.h"

namespace worlddirect {

  constexpr const char* KEY_ICCID = "iccid";
  // constexpr const char* KEY_FIRMWARE_NAME = "firmware_name";
  // constexpr const char* KEY_FIRMWARE_VERSION = "firmware_version";
  // constexpr const char* KEY_MANUFACTURE_DATE = "manufacture_date";

  DeviceProvisioning::DeviceProvisioning(QObject *parent) :
    QObject(parent),
    m_token()
  {

  }

  void DeviceProvisioning::getToken()
  {
    emit printData("Requesting Identity Token...");
    auto thread = QThread::create(
          [this]{
        QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);
        auto server = settings.value(KEY_IDENTITY_SERVER).toString().toStdString();
        auto client = settings.value(KEY_IDENTITY_CLIENT).toString().toStdString();
        auto secret = settings.value(KEY_IDENTITY_SECRET).toString().toStdString();
        auto scope = settings.value(KEY_IDENTITY_SCOPE).toString().toStdString();

        worlddirect::IdentityClient ident(server);
        auto token = QString::fromStdString(ident.RequestToken(client, secret, scope));

        if(ident.hadError()){
            auto errmsg = QString::fromStdString(ident.errorMessage());
            emit errorMessage(errmsg);
            return;
          }

        setToken(token);
        emit successMessage("Token successfully requested");
      }
    );

    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    thread->start();
  }

  void DeviceProvisioning::downloadLatestFirmware()
  {
    emit printData("Downloading latest Firmware...");
    auto thread = QThread::create(
          [this]{
        QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);
        auto server = settings.value(KEY_PROVISIONING_SERVER).toString().toStdString();
        auto path = settings.value(KEY_FLASHER_PATHTOFW).toString().toStdString();
        auto name = settings.value(KEY_PROVISIONING_NAME).toString().toStdString();
        auto token = m_token.toStdString();

        worlddirect::DeviceProvisioningAPIClient cli(server);

        cli.SetAccessToken(token);
        if(cli.hadError()){
            auto errmsg = QString::fromStdString(cli.errorMessage());
            emit errorMessage(errmsg);
            return;
          }

        cli.FirmwareGet(name, path);
        if(cli.hadError()){
            auto errmsg = QString::fromStdString(cli.errorMessage());
            emit errorMessage(errmsg);
            return;
          }

        emit successMessage("latest Firmware successfully downloaded");
      }
    );

    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    thread->start();

  }

  void DeviceProvisioning::createDevice(const QString &epName, const QString &type, const QString &hwVer, const QString &iccId)
  {
    emit printData("Registering Device...");
    auto thread = QThread::create(
          [this, epName, type, hwVer, iccId]{
        QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);
        auto server = settings.value(KEY_PROVISIONING_SERVER).toString().toStdString();
        auto path = settings.value(KEY_FLASHER_PATHTOFW).toString().toStdString();
        auto name = settings.value(KEY_PROVISIONING_NAME).toString().toStdString();
        auto token = m_token.toStdString();

        auto id = epName.toStdString();
        auto ty = type.toStdString();
        auto ver = hwVer.toStdString();
        auto icc = iccId.toStdString();

        worlddirect::DeviceProvisioningAPIClient cli(server);
        cli.SetAccessToken(token);
        if(cli.hadError()){
            auto errmsg = QString::fromStdString(cli.errorMessage());
            emit errorMessage(errmsg);
            return;
          }

        cli.DevicesPost(id, ty, ver, {{KEY_ICCID, icc}});
        if(cli.hadError()){
            auto errmsg = QString::fromStdString(cli.errorMessage());
            emit errorMessage(errmsg);
            return;
          }

        emit successMessage("Device successfully registered");
      }
    );

    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    thread->start();
  }

  void DeviceProvisioning::getPsk(const QString &epName)
  {
    emit printData("Requesting PSK for Device...");
    auto thread = QThread::create(
          [this, epName]{
        // need epname, key and value
        QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);
        auto server = settings.value(KEY_PROVISIONING_SERVER).toString().toStdString();
        auto token = m_token.toStdString();

        auto id = epName.toStdString();

        // authenticate
        worlddirect::DeviceProvisioningAPIClient cli(server);
        cli.SetAccessToken(token);
        if(cli.hadError()){
            auto errmsg = QString::fromStdString(cli.errorMessage());
            emit errorMessage(errmsg);
            return;
          }

        // getPSK
        auto key = cli.DevicesByIdPskGet(id);
        if(cli.hadError()){
            auto errmsg = QString::fromStdString(cli.errorMessage());
            emit errorMessage(errmsg);
            return;
          }

        auto psk = QString::fromStdString(key);
        emit pskReceived(psk);
        emit successMessage("PSK received");
      }
    );

    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    thread->start();
  }

  void DeviceProvisioning::validatePsk(const QString &epName, const QString &iccId, const QString &psk)
  {
    emit printData("Validating PSK...");
    auto thread = QThread::create(
          [this, epName, iccId, psk]{
        // need epname, key and value
        QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);
        auto server = settings.value(KEY_PROVISIONING_SERVER).toString().toStdString();
        auto token = m_token.toStdString();

        auto id = epName.toStdString();
        auto key = psk.toStdString();
        auto icc = iccId.toStdString();

        worlddirect::DeviceProvisioningAPIClient cli(server);
        cli.SetAccessToken(token);
        if(cli.hadError()){
            auto errmsg = QString::fromStdString(cli.errorMessage());
            emit errorMessage(errmsg);
            return;
          }

        //encrypt iccid
        MbedCrypto aes_128_ccm_8;
        std::string _plaintext(KEY_ICCID);
        std::string _cyphertext_sent;
        aes_128_ccm_8.encryptWithSymmetricCiphersBase64(_plaintext, key, _cyphertext_sent);
        if(aes_128_ccm_8.hadError()){
            auto errmsg = QString::fromStdString(aes_128_ccm_8.errorMessage());
            emit errorMessage(errmsg);
            return;
          }
        std::cout<<_cyphertext_sent<<std::endl;

        //validate with encrypted key
        auto _cypehertext_rec = cli.DevicesByIdValidateEncryptionGet(id, _cyphertext_sent);
        if(cli.hadError()){
            auto errmsg = QString::fromStdString(cli.errorMessage());
            emit errorMessage(errmsg);
            return;
          }

        //decrypt answer
        _plaintext.clear();
        aes_128_ccm_8.decryptWithSymmetricCiphersBase64(_cypehertext_rec, key, _plaintext);
        if(aes_128_ccm_8.hadError()){
            auto errmsg = QString::fromStdString(aes_128_ccm_8.errorMessage());
            emit errorMessage(errmsg);
            return;
          }

        if(_plaintext.compare(icc) != 0){
            emit errorMessage("PSK validation failed value received did not match");
            return;
          }

        emit successMessage("PSK validated");
      }
    );

    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    thread->start();

  }

  void DeviceProvisioning::setToken(const QString &s)
  {
    m_token = s;
  }

} // namespace worlddirect
