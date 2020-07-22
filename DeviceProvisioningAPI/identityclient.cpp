#include "identityclient.h"

#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "httpcodeutils.h"

namespace worlddirect {

  constexpr const char* S_CLIENT_ID_KEY = "client_id";
  constexpr const char*  S_SCOPE_KEY = "scope";
  constexpr const char* S_CLIENT_SECRET_KEY = "client_secret";
  constexpr const char* S_GRANT_TYPE_KEY = "grant_type";
  constexpr const char* S_GRANT_TYPE = "client_credentials";

  constexpr const char* ACCESS_TOKEN = "access_token";
  // constexpr const char* EXPIRES_IN   = "expires_in";
  // constexpr const char* TOKEN_TYPE   = "token_type";

  IdentityClient::IdentityClient(const std::string &identiyUrl):
    m_stringBuffer(),
    m_curl(nullptr),
    m_headers(nullptr),
    m_lastCurlCode(CURLE_OK),
    m_lastHttpCode(HTTP_OK),
    m_lastErrorMessage()
  {
    m_stringBuffer.str("");
    m_stringBuffer.clear();

    curl_global_init(CURL_GLOBAL_ALL);

    m_curl = curl_easy_init();

    //curl_easy_setopt(m_curl, CURLOPT_VERBOSE, true);

    curl_easy_setopt(m_curl, CURLOPT_URL, identiyUrl.c_str());

    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, IdentityClient::curlWriteFuncCB);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);

    m_headers = curl_slist_append(m_headers, "Content-Type: application/x-www-form-urlencoded");
    m_headers = curl_slist_append(m_headers, "Cache-Control: no - cache");
    if (m_headers == NULL){
        std::cerr << "curl_slist_append() failed: " << std::endl;
        m_lastErrorMessage = "curl_slist_append() failed:";
        return;
      }

    /* set our custom set of headers */
    m_lastCurlCode = curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headers);
    if (m_lastCurlCode != CURLE_OK) {
        setCurlError();
      }
  }

  IdentityClient::~IdentityClient()
  {
    curl_slist_free_all(m_headers); /* free the header list */
    m_headers = nullptr;

    curl_easy_cleanup(m_curl);
    m_curl = nullptr;
  }

  Token IdentityClient::RequestToken(const std::string &clientId, const std::string &clientSecret, const std::string &scope)
  {
    m_lastCurlCode = CURLE_OK;
    m_lastHttpCode = HTTP_OK;

    std::stringstream stringBuilder;
    stringBuilder << S_CLIENT_ID_KEY     << "=" << clientId << "&";
    stringBuilder << S_SCOPE_KEY         << "=" << scope << "&";
    stringBuilder << S_CLIENT_SECRET_KEY << "=" << clientSecret << "&";
    stringBuilder << S_GRANT_TYPE_KEY    << "=" << S_GRANT_TYPE;
    auto postfields = stringBuilder.str();

    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, postfields.c_str());
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, (long)postfields.size());
    m_stringBuffer.str("");
    m_stringBuffer.clear();

    m_lastCurlCode = curl_easy_perform(m_curl);
    if (m_lastCurlCode != CURLE_OK) {
        setCurlError();
        return Token();
      }

    curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &m_lastHttpCode);

    if (m_lastHttpCode != HTTP_OK){
        setHttpError();
        return Token();
      }


    boost::property_tree::ptree resp;
    // Parse the JSON into the property tree.
    boost::property_tree::read_json(m_stringBuffer, resp);
    auto token = resp.get<std::string>(ACCESS_TOKEN);
    return Token(token);

  }

  bool IdentityClient::hadError() const
  {
    return (m_lastCurlCode != CURLE_OK) || (m_lastHttpCode != HTTP_OK);
  }

  std::string IdentityClient::errorMessage() const
  {
    return m_lastErrorMessage;
  }

  size_t IdentityClient::curlWriteFuncCB(char *ptr, size_t size, size_t nmemb, IdentityClient *_this)
  {
    if (_this == nullptr) {
        return 0;
      }
    return _this->curlWriteFunc(ptr, size, nmemb);
  }

  size_t IdentityClient::curlWriteFunc(char *ptr, size_t size, size_t nmemb)
  {
    m_stringBuffer.write(ptr, size * nmemb);
    return size * nmemb;
  }

  void IdentityClient::setCurlError()
  {
    std::stringstream stringBuilder;
    stringBuilder << "CURL error " << curl_easy_strerror(m_lastCurlCode);

    m_lastErrorMessage = stringBuilder.str();
  }

  void IdentityClient::setHttpError()
  {
    std::stringstream stringBuilder;
    stringBuilder << "HTTP error " << code2status(m_lastHttpCode);

    m_lastErrorMessage = stringBuilder.str();

  }


} // namespace worlddirect
