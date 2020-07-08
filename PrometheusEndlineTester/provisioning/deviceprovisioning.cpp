#include "deviceprovisioning.h"

#include <QSettings>
#include <QThread>
#include <QDebug>

#include <identityclient.h>
#include <deviceprovisioningapiclient.h>

#include "PrometheusEndlineTester_global.h"

namespace worlddirect {

  DeviceProvisioning::DeviceProvisioning(QObject *parent) :
    QObject(parent),
    m_token()
  {

  }

  void DeviceProvisioning::getToken()
  {
    auto thread = QThread::create(
          [this]{
        QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);
        auto server = settings.value(KEY_IDENTITY_SERVER).toString().toStdString();
        auto client = settings.value(KEY_IDENTITY_CLIENT).toString().toStdString();
        auto secret = settings.value(KEY_IDENTITY_SECRET).toString().toStdString();
        auto scope = settings.value(KEY_IDENTITY_SCOPE).toString().toStdString();

        worlddirect::IdentityClient ident(server);
        auto token = QString::fromStdString(ident.RequestToken(client, secret, scope));

        emit resultReady(token);
        emit printData("Token successfully requested");
      }
    );

    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    connect(this, &DeviceProvisioning::resultReady, this, &DeviceProvisioning::setToken);
    thread->start();
  }

  void DeviceProvisioning::downloadLatestFirmware()
  {
    auto thread = QThread::create(
          [this]{
        QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);
        auto server = settings.value(KEY_PROVISIONING_SERVER).toString().toStdString();
        auto path = settings.value(KEY_FLASHER_PATHTOFW).toString().toStdString();
        auto name = settings.value(KEY_PROVISIONING_NAME).toString().toStdString();
        auto token = m_token.toStdString();

        worlddirect::DeviceProvisioningAPIClient cli(server);
        cli.SetAccessToken(token);
        cli.FirmwareGet(name, path);

        emit printData("Firmware downloaded");

      }
    );

    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    connect(this, &DeviceProvisioning::resultReady, this, &DeviceProvisioning::setToken);
    thread->start();

  }

  void DeviceProvisioning::setToken(const QString &s)
  {
    m_token = s;
  }

} // namespace worlddirect
