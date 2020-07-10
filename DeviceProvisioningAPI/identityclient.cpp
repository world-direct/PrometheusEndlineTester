#include "identityclient.h"

#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

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
    m_headers(nullptr)
  {
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
        return;
      }

    /* set our custom set of headers */
    auto res = curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headers);
    if (res != CURLE_OK) {
        std::cerr << __PRETTY_FUNCTION__ << " curl_easy_setopt() failed: " << curl_easy_strerror(res) << std::endl;
      }

    m_stringBuffer.str("");
    m_stringBuffer.clear();
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
    CURLcode res;

    res = curl_easy_perform(m_curl);
    if (res != CURLE_OK) {
        std::cerr << __PRETTY_FUNCTION__ << " curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return Token();
      }

    long http_code;
    curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (http_code != 200 && http_code != 204)
      {
        std::cerr << __PRETTY_FUNCTION__ << "The HTTP status code of the response was not expected (" << http_code << ")" << std::endl;
        return Token();
      }

    if(http_code != 200){
        return Token();
      }

    boost::property_tree::ptree resp;
    // Parse the JSON into the property tree.
    boost::property_tree::read_json(m_stringBuffer, resp);
    auto token = resp.get<std::string>(ACCESS_TOKEN);
    return Token(token);

  }

  size_t IdentityClient::curlWriteFuncCB(char *ptr, size_t size, size_t nmemb, IdentityClient *_this)
  {
    if (_this == nullptr) {
        std::cerr << __PRETTY_FUNCTION__ << "discoveryClient is null" << std::endl;
        return 0;
      }
    return _this->curlWriteFunc(ptr, size, nmemb);
  }

  size_t IdentityClient::curlWriteFunc(char *ptr, size_t size, size_t nmemb)
  {
    m_stringBuffer.write(ptr, size * nmemb);
    return size * nmemb;
  }


} // namespace worlddirect
