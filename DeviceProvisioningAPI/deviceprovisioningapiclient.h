#ifndef WORLDDIRECT_DEVICEPROVISIONINGAPICLIENT_H
#define WORLDDIRECT_DEVICEPROVISIONINGAPICLIENT_H

#include <string>
#include <map>
#include <sstream>

#include <curl/curl.h>
#include <curl/easy.h>

namespace worlddirect {

  class DeviceProvisioningAPIClient
  {
  public:
   static const std::string S_DEFAULT_TOKEN_TYPE;

  public:
    DeviceProvisioningAPIClient(const std::string& apiUrl);
    DeviceProvisioningAPIClient&  operator= ( const  DeviceProvisioningAPIClient& ) = delete;
    DeviceProvisioningAPIClient&  operator= ( const  DeviceProvisioningAPIClient&& ) = delete;

    DeviceProvisioningAPIClient ( const  DeviceProvisioningAPIClient& ) = delete;
    DeviceProvisioningAPIClient ( const  DeviceProvisioningAPIClient&& ) = delete;
    ~DeviceProvisioningAPIClient() = default;

  public:

    void SetAccessToken(const std::string& accessToken, const std::string& tokenType = S_DEFAULT_TOKEN_TYPE);

    void DevicesPost(const std::string& id, const std::string& type, const std::string& version, const std::map<std::string, std::string>& metadata);
    std::string DevicesByIdPskGet(const std::string& id);
    std::string DevicesByIdValidateEncryptionGet(const std::string& id, const std::string& message);

    void FirmwareGet();

  private:
    static size_t curlWriteFuncCB(char *ptr, size_t size, size_t nmemb, std::stringstream * in);
    static size_t curlReadFuncCB(char *ptr, size_t size, size_t nmemb, std::stringstream * out);

  private:
    std::stringstream m_stringBuffer;
    CURL * m_curl;
    struct curl_slist *m_headers;
    std::string m_apiUrl;

  };

} // namespace worlddirect

#endif // WORLDDIRECT_DEVICEPROVISIONINGAPICLIENT_H
