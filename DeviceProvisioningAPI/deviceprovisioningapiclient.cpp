#include "deviceprovisioningapiclient.h"

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "httpcodeutils.h"

namespace worlddirect {

  constexpr const char* S_API = "api";
  constexpr const char* S_API_VERSION = "v2";
  constexpr const char* S_REGISTRATION = "registration";
  constexpr const char* S_GET_PSK = "getPsk";
  constexpr const char* S_VALIDATE_ENCRYPTION = "validateEncryption";
  constexpr const char* S_MESSAGE = "message";
  constexpr const char* S_DOWNLOAD_FIRMWARE = "downloadFirmware";
  constexpr const char* S_NAME = "name";
  constexpr const char* S_BEGIN = "begin";
  constexpr const char* S_COMPLETE = "complete";

  const std::string DeviceProvisioningAPIClient::S_DEFAULT_TOKEN_TYPE = "Bearer";

  DeviceProvisioningAPIClient::DeviceProvisioningAPIClient(const std::string &apiUrl):
    m_stringBuffer(),
    m_curl(nullptr),
    m_headers(nullptr),
    m_apiUrl(apiUrl),
    m_lastCurlCode(CURLE_OK),
    m_lastHttpCode(HTTP_OK),
    m_lastErrorMessage()
  {
    curl_global_init(CURL_GLOBAL_ALL);

    m_curl = curl_easy_init();


    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, DeviceProvisioningAPIClient::curlWriteFuncCB);
    m_headers = curl_slist_append(m_headers, "Content-Type: application/json");
    m_headers = curl_slist_append(m_headers, "Cache-Control: no - cache");

    m_lastCurlCode = curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headers);
    if (m_lastCurlCode != CURLE_OK) {
        // std::cerr << __PRETTY_FUNCTION__ << " curl_easy_setopt() failed: " << curl_easy_strerror(res) << std::endl;
        setCurlError();
      }
  }

  void DeviceProvisioningAPIClient::SetAccessToken(const std::string &accessToken, const std::string &tokenType)
  {
    resetAllErrors();

    if (accessToken.empty() == true) {
        m_lastHttpCode = HTTP_UNAUTHORIZED;
        setHttpError();
        return;
      }

    std::string m_accessToken;
    m_accessToken.clear();
    std::stringstream stringBuilder;
    stringBuilder << "Authorization: ";
    if (tokenType.length() > 0) {
        stringBuilder << tokenType;
      }
    else {
        stringBuilder << S_DEFAULT_TOKEN_TYPE;
      }

    if (accessToken.length() > 0)
      {
        stringBuilder << " " << accessToken;
      }

    m_accessToken = stringBuilder.str();
    m_headers = curl_slist_append(m_headers, m_accessToken.c_str());
    m_lastCurlCode = curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headers);
    if (m_lastCurlCode != CURLE_OK) {
        setCurlError();
      }
  }

  void DeviceProvisioningAPIClient::RegistrationBeginPost(const std::string &id, const std::string &type, const std::string &version, const std::map<std::string, std::string> &metadata)
  {
    resetAllErrors();

    std::stringstream stringBuilder;
    stringBuilder << m_apiUrl << "/" << S_API << "/" << S_API_VERSION << "/" << S_REGISTRATION << "/" << S_BEGIN;
    auto url = stringBuilder.str();

    boost::property_tree::ptree pf;
    pf.put("id", id);
    pf.put("hardwareType", type);
    pf.put("hardwareVersion", version);

    boost::property_tree::ptree md;
    for(auto i: metadata) {
        boost::property_tree::ptree meta;
        meta.put("key", i.first);
        meta.put("value", i.second);
        md.push_back(std::make_pair("", meta));
      }
    pf.add_child("parameters", md);

    std::stringstream stringBufferPf;
    boost::property_tree::write_json(stringBufferPf, pf);
    auto postfields = stringBufferPf.str();

    boost::property_tree::ptree resp;
    boost::property_tree::ptree clresp;

    auto curl = curl_easy_duphandle(m_curl);

    std::stringstream stringBufferIn;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stringBufferIn);

    stringBufferIn.str("");
    stringBufferIn.clear(); // Clear state flags.

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    if (postfields.size() > 0) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)postfields.size());
      }
    else {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 0L);
      }

    m_lastCurlCode = curl_easy_perform(curl);
    if (m_lastCurlCode != CURLE_OK)
      {
        setCurlError();
        curl_easy_cleanup(curl);
        return ;
      }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &m_lastHttpCode);

    if (m_lastHttpCode != HTTP_CREATED)
      {
        setHttpError(stringBufferIn.str());
        curl_easy_cleanup(curl);
        return;
      }

    curl_easy_cleanup(curl);
    curl = nullptr;

    return;
  }

  std::string DeviceProvisioningAPIClient::RegistrationByIdGetPskGet(const std::string &id)
  {
    resetAllErrors();

    std::stringstream stringBuilder;
    stringBuilder << m_apiUrl << "/" << S_API << "/" << S_API_VERSION << "/" << S_REGISTRATION << "/" << id << "/" << S_GET_PSK;
    auto url = stringBuilder.str();

    auto curl = curl_easy_duphandle(m_curl);

    std::stringstream stringBufferIn;

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stringBufferIn);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    stringBufferIn.str("");
    stringBufferIn.clear(); // Clear state flags.

    m_lastCurlCode = curl_easy_perform(curl);
    if (m_lastCurlCode != CURLE_OK) {
        setCurlError();
        curl_easy_cleanup(curl);
        return std::string();
      }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &m_lastHttpCode);

    if (m_lastHttpCode != HTTP_OK){
        setHttpError(stringBufferIn.str());
        curl_easy_cleanup(curl);
        return std::string();
      }

    curl_easy_cleanup(curl);
    return stringBufferIn.str();
  }

  std::string DeviceProvisioningAPIClient::RegistrationByIdValidateEncryptionGet(const std::string &id, const std::string &message)
  {
    resetAllErrors();
    auto curl = curl_easy_duphandle(m_curl);

    auto msg_enc = curl_easy_escape(curl, message.c_str(), message.size() );
    std::stringstream stringBuilder;
    stringBuilder << m_apiUrl << "/" << S_API << "/" << S_API_VERSION << "/" << S_REGISTRATION << "/" << id << "/" << S_VALIDATE_ENCRYPTION << "?" << S_MESSAGE <<"="<< msg_enc;
    auto url = stringBuilder.str();

    std::stringstream stringBufferIn;


    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stringBufferIn);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    stringBufferIn.str("");
    stringBufferIn.clear(); // Clear state flags.

    m_lastCurlCode = curl_easy_perform(curl);
    if (m_lastCurlCode != CURLE_OK)
      {
        setCurlError();
        curl_easy_cleanup(curl);
        return std::string();
      }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &m_lastHttpCode);

    if (m_lastHttpCode != HTTP_OK)
      {
        setHttpError(stringBufferIn.str());
        curl_easy_cleanup(curl);
        return std::string();
      }

    curl_easy_cleanup(curl);
    return stringBufferIn.str();
  }

  void DeviceProvisioningAPIClient::RegistartionDownloadFirmwareGet(const std::string &name, const std::string &filename)
  {
    resetAllErrors();

    std::stringstream stringBuilder;
    stringBuilder << m_apiUrl << "/" << S_API << "/" << S_API_VERSION << "/" << S_REGISTRATION << "/" << S_DOWNLOAD_FIRMWARE << "?" << S_NAME <<"="<< name;;
    auto url = stringBuilder.str();

    auto curl = curl_easy_duphandle(m_curl);

    std::stringstream stringBufferIn;

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stringBufferIn);

    stringBufferIn.str("");
    stringBufferIn.clear(); // Clear state flags.

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

    std::fstream outfile;

    outfile.open(filename, std::ios::out | std::ios::trunc);
    if(outfile.is_open() == false){
        stringBuilder.clear();
        stringBuilder << "error could not open file: " << filename;

        m_lastErrorMessage = stringBuilder.str();

        return;
      }

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outfile);
    m_lastCurlCode = curl_easy_perform(curl);
    if (m_lastCurlCode != CURLE_OK){
        setCurlError();
        outfile.close();
        curl_easy_cleanup(curl);
        return;
      }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &m_lastHttpCode);

    if (m_lastHttpCode != HTTP_OK){
        setHttpError(stringBufferIn.str());
        outfile.close();
        curl_easy_cleanup(curl);
        return;
      }

    outfile.close();
    curl_easy_cleanup(curl);
    return;

  }

  void DeviceProvisioningAPIClient::RegistrationByIdCompletePut(const std::string &id)
  {

    std::stringstream stringBuilder;
    stringBuilder << m_apiUrl << "/" << S_API << "/" << S_API_VERSION << "/" << S_REGISTRATION << "/" << id << "/" << S_COMPLETE;
    auto url = stringBuilder.str();

    auto curl = curl_easy_duphandle(m_curl);
    std::stringstream stringBufferIn;

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stringBufferIn);

    stringBufferIn.str("");
    stringBufferIn.clear(); // Clear state flags.

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_PUT, 1L);

    m_lastCurlCode = curl_easy_perform(curl);
    if (m_lastCurlCode != CURLE_OK){
        setCurlError();
        curl_easy_cleanup(curl);
        return;
      }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &m_lastHttpCode);
    if (m_lastHttpCode != HTTP_OK){
        setHttpError(stringBufferIn.str());
        curl_easy_cleanup(curl);
        return;
      }

    curl_easy_cleanup(curl);
    return;
  }

  bool DeviceProvisioningAPIClient::hadError() const
  {
    return (m_lastCurlCode != CURLE_OK) || ((m_lastHttpCode != HTTP_OK) && (m_lastHttpCode != HTTP_CREATED)) || (m_lastErrorMessage.empty() == false);
  }

  std::string DeviceProvisioningAPIClient::errorMessage() const
  {
    return m_lastErrorMessage;
  }

  size_t DeviceProvisioningAPIClient::curlWriteFuncCB(char *ptr, size_t size, size_t nmemb, std::iostream *in)
  {
    if (in == nullptr) {
        return 0;
      }
    in->write(ptr, size*nmemb);
    return size * nmemb;
  }

  size_t DeviceProvisioningAPIClient::curlReadFuncCB(char *ptr, size_t size, size_t nmemb, std::stringstream *out)
  {
    if (out == nullptr) {
        return 0;
      }
    out->read(ptr,size*nmemb);
    return out->gcount();
  }

  void DeviceProvisioningAPIClient::setCurlError()
  {
    std::stringstream stringBuilder;
    stringBuilder << "CURL error " << curl_easy_strerror(m_lastCurlCode);

    m_lastErrorMessage = stringBuilder.str();
  }

  void DeviceProvisioningAPIClient::setHttpError(const std::string& msg)
  {
    std::stringstream stringBuilder;
    stringBuilder << "HTTP error " << code2status(m_lastHttpCode);
    if(msg.empty() == false){
        stringBuilder << std::endl;
        stringBuilder << msg;
        stringBuilder << std::endl;
      }

    m_lastErrorMessage = stringBuilder.str();
  }

  void DeviceProvisioningAPIClient::resetAllErrors()
  {
    m_lastCurlCode = CURLE_OK;
    m_lastHttpCode = HTTP_OK;
    m_lastErrorMessage.clear();
  }

} // namespace worlddirect
