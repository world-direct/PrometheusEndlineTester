#include "deviceprovisioningapiclient.h"

#include<iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


namespace worlddirect {

  constexpr const char* S_DEVICES = "devices";
  constexpr const char* S_PSK = "psk";
  constexpr const char* S_VALIDATE_ENCRYPTION = "validateEncryption";
  constexpr const char* S_MESSAGE = "message";

  const std::string DeviceProvisioningAPIClient::S_DEFAULT_TOKEN_TYPE = "Bearer";

  DeviceProvisioningAPIClient::DeviceProvisioningAPIClient(const std::string &apiUrl):
    m_stringBuffer(),
    m_curl(nullptr),
    m_headers(nullptr),
    m_apiUrl(apiUrl)
  {
    curl_global_init(CURL_GLOBAL_ALL);

    m_curl = curl_easy_init();


    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, DeviceProvisioningAPIClient::curlWriteFuncCB);
    m_headers = curl_slist_append(m_headers, "Content-Type: application/json");
    m_headers = curl_slist_append(m_headers, "Cache-Control: no - cache");

    auto res = curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headers);
    if (res != CURLE_OK) {
        std::cerr << __PRETTY_FUNCTION__ << " curl_easy_setopt() failed: " << curl_easy_strerror(res) << std::endl;
      }
  }

  void DeviceProvisioningAPIClient::SetAccessToken(const std::string &accessToken, const std::string &tokenType)
  {
    if (accessToken.empty() == true) {
        std::cerr << __PRETTY_FUNCTION__ << " Access Token is empty " << std::endl;
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
    auto res = curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headers);
    if (res != CURLE_OK) {
        std::cerr << __PRETTY_FUNCTION__ << " curl_easy_setopt() failed: " << curl_easy_strerror(res) << std::endl;
      }
  }

  void DeviceProvisioningAPIClient::DevicesPost(const std::string &id, const std::string &type, const std::string &version, const std::map<std::string, std::string> &metadata)
  {
    std::stringstream stringBuilder;
    stringBuilder << m_apiUrl << "/" << S_DEVICES;
    auto url = stringBuilder.str();

    boost::property_tree::ptree pf;
    pf.put("id", id);
    pf.put("type", type);
    pf.put("version", version);

    boost::property_tree::ptree md;
    for(auto i: metadata) {
        boost::property_tree::ptree meta;
        meta.put("key", i.first);
        meta.put("value", i.second);
        md.push_back(std::make_pair("", meta));
      }
    pf.add_child("metadata", md);

    std::stringstream stringBufferPf;
    boost::property_tree::write_json(stringBufferPf, pf);
    auto postfields = stringBufferPf.str();


    CURLcode res;
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

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      {
        std::cerr << __PRETTY_FUNCTION__ <<url<< " curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return ;
      }

    if (CURLE_OK != res)
      {
        curl_easy_cleanup(curl);
        return;
      }

    long http_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (http_code != 201 && http_code != 204)
      {
        std::cerr << __PRETTY_FUNCTION__ << "The HTTP status code of the response was not expected (" << http_code << ")" << std::endl;
        curl_easy_cleanup(curl);
        return;
      }

    if (http_code != 201)
      {
        curl_easy_cleanup(curl);
        return;
      }

    if (stringBufferIn.gcount() <= 0)
      {
        curl_easy_cleanup(curl);
        return;
      }

    boost::property_tree::ptree content;
    // Parse the JSON into the property tree.
    boost::property_tree::read_json(stringBufferIn, content);


    resp = clresp;

    curl_easy_cleanup(curl);
    curl = nullptr;

    return;
  }

  std::string DeviceProvisioningAPIClient::DevicesByIdPskGet(const std::string &id)
  {

    std::stringstream stringBuilder;
    stringBuilder << m_apiUrl << "/" << S_DEVICES << "/" << id << "/" << S_PSK;
    auto url = stringBuilder.str();

    CURLcode res;

    auto curl = curl_easy_duphandle(m_curl);


    std::stringstream stringBufferIn;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stringBufferIn);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    stringBufferIn.str("");
    stringBufferIn.clear(); // Clear state flags.

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      {
        std::cerr << __PRETTY_FUNCTION__ <<url<< " curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return std::string();
      }

    if (CURLE_OK != res)
      {
        curl_easy_cleanup(curl);
        return std::string();
      }
    long http_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (http_code != 200 && http_code != 204)
      {
        std::cerr << url<< " The HTTP status code of the response was not expected (" << http_code << ")" << std::endl;
        curl_easy_cleanup(curl);
        return std::string();
      }

    if (http_code != 200) {
        curl_easy_cleanup(curl);
        return std::string();
      }

    curl_easy_cleanup(curl);
    return stringBufferIn.str();
  }

  std::string DeviceProvisioningAPIClient::DevicesByIdValidateEncryptionGet(const std::string &id, const std::string &message)
  {
    std::stringstream stringBuilder;
    stringBuilder << m_apiUrl << "/" << S_DEVICES << "/" << id << "/" << S_VALIDATE_ENCRYPTION << "?" << S_MESSAGE <<"="<< message;
    auto url = stringBuilder.str();

    CURLcode res;

    auto curl = curl_easy_duphandle(m_curl);


    std::stringstream stringBufferIn;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stringBufferIn);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    stringBufferIn.str("");
    stringBufferIn.clear(); // Clear state flags.

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      {
        std::cerr << __PRETTY_FUNCTION__ <<url<< " curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return std::string();
      }

    if (CURLE_OK != res)
      {
        curl_easy_cleanup(curl);
        return std::string();
      }
    long http_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (http_code != 200 && http_code != 204)
      {
        std::cerr << url<< " The HTTP status code of the response was not expected (" << http_code << ")" << std::endl;
        curl_easy_cleanup(curl);
        return std::string();
      }

    if (http_code != 200) {
        curl_easy_cleanup(curl);
        return std::string();
      }

    curl_easy_cleanup(curl);
    return stringBufferIn.str();
  }

  size_t DeviceProvisioningAPIClient::curlWriteFuncCB(char *ptr, size_t size, size_t nmemb, std::stringstream *in)
  {
    if (in == nullptr) {
        std::cerr << __PRETTY_FUNCTION__ << " discoveryClient is null" << std::endl;
        return 0;
      }
    in->write(ptr, size*nmemb);
    return size * nmemb;
  }

  size_t DeviceProvisioningAPIClient::curlReadFuncCB(char *ptr, size_t size, size_t nmemb, std::stringstream *out)
  {
    if (out == nullptr) {
        std::cerr << __PRETTY_FUNCTION__ << " discoveryClient is null" << std::endl;
        return 0;
      }
    out->read(ptr,size*nmemb);
    return out->gcount();
  }

} // namespace worlddirect
