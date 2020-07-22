#ifndef WORLDDIRECT_MBEDCRYPTO_H
#define WORLDDIRECT_MBEDCRYPTO_H

#include <vector>
#include <stdint.h>
#include <string>

#include "crypto_types.h"

namespace worlddirect {

  class MbedCrypto
  {
  public:
    MbedCrypto();
    MbedCrypto&  operator= ( const  MbedCrypto& ) = delete;
    MbedCrypto&  operator= ( const  MbedCrypto&& ) = delete;

    MbedCrypto ( const  MbedCrypto& ) = delete;
    MbedCrypto ( const  MbedCrypto&& ) = delete;
    ~MbedCrypto() = default;

  public:
    void encryptWithSymmetricCiphers(const std::vector<uint8_t>& _plaintext, const std::vector<uint8_t>& _key, std::vector<uint8_t> &_cyphertext);
    void decryptWithSymmetricCiphers(const std::vector<uint8_t> &_cyphertext, const std::vector<uint8_t> &_key, std::vector<uint8_t> &_plaintext);

    void encryptWithSymmetricCiphers(const std::string& _plaintext, const std::string& _key, std::string& _cyphertext);
    void decryptWithSymmetricCiphers(const std::string& _cyphertext, const std::string& _key, std::string& _plaintext);

    void encryptWithSymmetricCiphersBase64(const std::string& _plaintext, const std::string& _key, std::string& _cyphertext_b64);
    void decryptWithSymmetricCiphersBase64(const std::string& _cyphertext_b64, const std::string& _key, std::string& _plaintext);

    bool hadError()const;
    std::string errorMessage()const;

  private:
    void resetAllErrors();
    void setPsaError(std::string msg);
    std::string psacode2status(psa_status_t code);


  private:
    psa_status_t m_status;
    std::string m_lastErrorMessage;
  };

} // namespace worlddirect

#endif // WORLDDIRECT_MBEDCRYPTO_H
