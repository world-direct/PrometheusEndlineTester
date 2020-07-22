#ifndef WORLDDIRECT_IDENTITYCLIENT_H
#define WORLDDIRECT_IDENTITYCLIENT_H

#include <string>
#include <sstream>

#include <curl/curl.h>
#include <curl/easy.h>

#include "token.h"

namespace worlddirect {

  class IdentityClient
  {
  public:
    IdentityClient(const std::string& identiyUrl);
    IdentityClient&  operator= ( const  IdentityClient& ) = delete;
    IdentityClient&  operator= ( const  IdentityClient&& ) = delete;

    IdentityClient ( const  IdentityClient& ) = delete;
    IdentityClient ( const  IdentityClient&& ) = delete;
    ~IdentityClient();

  public:
    Token RequestToken(const std::string& clientId, const std::string& clientSecret, const std::string& scope );
    bool hadError()const;
    std::string errorMessage()const;

  private:
    static size_t curlWriteFuncCB(char *ptr, size_t size, size_t nmemb, IdentityClient *_this);
    size_t curlWriteFunc(char *ptr, size_t size, size_t nmemb);

    void setCurlError();
    void setHttpError();

  private:
    std::stringstream m_stringBuffer;
    CURL * m_curl;
    struct curl_slist *m_headers;

    CURLcode m_lastCurlCode;
    long m_lastHttpCode;

    std::string m_lastErrorMessage;
  };

} // namespace worlddirect

#endif // WORLDDIRECT_IDENTITYCLIENT_H
