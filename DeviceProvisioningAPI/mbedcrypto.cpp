#include "mbedcrypto.h"

#include "psa/crypto.h"
#include "psa/crypto_extra.h"
#include "mbedtls/version.h"

#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>


#include "base64.h"

namespace worlddirect {

  constexpr size_t tag_length = 8;
  constexpr auto alg = PSA_ALG_AEAD_WITH_TAG_LENGTH(PSA_ALG_CCM, tag_length);

  MbedCrypto::MbedCrypto()
  {

  }

  void MbedCrypto::encryptWithSymmetricCiphers(const std::vector<uint8_t> &_plaintext, const std::vector<uint8_t> &_key, std::vector<uint8_t> &_cyphertext)
  {
    psa_status_t status;

    auto key = _key.data();
    auto key_len = _key.size();

    constexpr size_t nonce_len = 12;
    uint8_t nonce[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};

    // const uint8_t additional_data[] = {0xEC, 0x46, 0xBB, 0x63, 0xB0, 0x25, 0x20, 0xC3, 0x3C, 0x49, 0xFD, 0x70};
    // size_t additional_data_len = sizeof (additional_data);
    uint8_t* additional_data = nullptr;
    size_t additional_data_len = 0;

    auto input_data_len = _plaintext.size();
    auto input_data = _plaintext.data();

    uint8_t *output_data = NULL;
    size_t output_size = 0;
    size_t output_length = 0;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_handle_t handle;

    // printf("Authenticate encrypt...\t");
    fflush(stdout);

    /* Initialize PSA Crypto */
    status = psa_crypto_init();
    if (status != PSA_SUCCESS) {
        // printf("Failed to initialize PSA Crypto\n");
        return;
      }

    output_size = input_data_len + tag_length;
    output_data = (uint8_t *)malloc(output_size);
    if (!output_data) {
        // printf("Out of memory\n");
        return;
      }

    /* Import a key */
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT);
    // psa_set_key_algorithm(&attributes, PSA_ALG_CCM);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attributes, 128);
    status = psa_import_key(&attributes, key, key_len, &handle);
    if (status != PSA_SUCCESS) {
        // printf("Failed to import a key\n");
        return;
      }
    psa_reset_key_attributes(&attributes);

    /* Authenticate and encrypt */
    status = psa_aead_encrypt(handle, alg,
                              nonce, nonce_len,
                              additional_data, additional_data_len,
                              input_data, input_data_len,
                              output_data, output_size,
                              &output_length);
    if (status != PSA_SUCCESS) {
        // printf("Failed to authenticate and encrypt\n");
        return;
      }

    // printf("Authenticated and encrypted\n");

    _cyphertext.clear();
    _cyphertext.reserve(nonce_len + output_length);
    _cyphertext.assign(nonce, nonce+nonce_len);
    _cyphertext.insert(_cyphertext.end(), output_data, output_data+output_length);
    // _cyphertext.assign(output_data, output_data+output_size);
    /* Clean up */
    free(output_data);

    /* Destroy the key */
    psa_destroy_key(handle);

    mbedtls_psa_crypto_free();
  }

  //AES_128_CCM_8
  void worlddirect::MbedCrypto::decryptWithSymmetricCiphers(const std::vector<uint8_t> &_cyphertext, const std::vector<uint8_t> &_key,  std::vector<uint8_t> &_plaintext)
  {
    psa_status_t status;

    auto key = _key.data();
    auto key_len = _key.size();

    constexpr size_t nonce_len = 12;
    uint8_t nonce[nonce_len] = {0};
    memcpy_s(nonce, nonce_len, _cyphertext.data(), nonce_len);

    uint8_t* additional_data = nullptr;
    size_t additional_data_len = 0;

    auto input_data_len = _cyphertext.size() - (nonce_len);
    auto input_data = _cyphertext.data()+nonce_len;

    uint8_t *output_data = NULL;
    size_t output_size = 0;
    size_t output_length = 0;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_handle_t handle;


    fflush(stdout);

    /* Initialize PSA Crypto */
    status = psa_crypto_init();
    if (status != PSA_SUCCESS) {
        // printf("Failed to initialize PSA Crypto\n");
        return;
      }

    output_size = input_data_len;
    output_data = (uint8_t *)malloc(output_size);
    if (!output_data) {
        // printf("Out of memory\n");
        return;
      }

    /* Import a key */
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DECRYPT);
    // psa_set_key_algorithm(&attributes, PSA_ALG_CCM);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attributes, 128);
    status = psa_import_key(&attributes, key, key_len, &handle);
    if (status != PSA_SUCCESS) {
        // printf("Failed to import a key\n");
        return;
      }
    psa_reset_key_attributes(&attributes);

    status = psa_aead_decrypt(handle, alg,
                              nonce, nonce_len,
                              additional_data, additional_data_len,
                              input_data, input_data_len,
                              output_data, output_size,
                              &output_length);
    if (status != PSA_SUCCESS) {
        // printf("Failed to authenticate and decrypt\n");
        return;
      }

    // printf("Authenticated and decrypted\n");

    _plaintext.clear();
    _plaintext.resize(output_length);
    _plaintext.assign(output_data, output_data+output_length);

    /* Clean up */
    free(output_data);

    /* Destroy the key */
    psa_destroy_key(handle);

    mbedtls_psa_crypto_free();

  }

  void MbedCrypto::encryptWithSymmetricCiphers(const std::string &_plaintext, const std::string &_key, std::string &_cyphertext)
  {

      std::vector<uint8_t> message_plaintext(_plaintext.begin(), _plaintext.end());
      std::vector<uint8_t> message_key(_key.begin(), _key.end());

      std::vector<uint8_t> message_cyphertext;
      encryptWithSymmetricCiphers(message_plaintext, message_key, message_cyphertext);
      _cyphertext.clear();
      _cyphertext.reserve(message_cyphertext.size());
      _cyphertext.assign(message_cyphertext.begin(), message_cyphertext.end());

  }

  void MbedCrypto::decryptWithSymmetricCiphers(const std::string &_cyphertext, const std::string &_key, std::string &_plaintext)
  {
      std::vector<uint8_t> message_cyphertext(_cyphertext.begin(), _cyphertext.end());
      std::vector<uint8_t> message_key(_key.begin(), _key.end());

      std::vector<uint8_t> message_plaintext;
      decryptWithSymmetricCiphers(message_cyphertext, message_key, message_plaintext);
      _plaintext.clear();
      _plaintext.reserve(message_cyphertext.size());
      _plaintext.assign(message_plaintext.begin(), message_plaintext.end());
  }

  void MbedCrypto::encryptWithSymmetricCiphersBase64(const std::string &_plaintext, const std::string &_key, std::string &_cyphertext_b64)
  {
      std::vector<uint8_t> message_plaintext(_plaintext.begin(), _plaintext.end());
      std::vector<uint8_t> message_key(_key.begin(), _key.end());

      std::vector<uint8_t> message_cyphertext;
      encryptWithSymmetricCiphers(message_plaintext, message_key, message_cyphertext);

      _cyphertext_b64 =  Base64::Encode(message_cyphertext);
  }

  void MbedCrypto::decryptWithSymmetricCiphersBase64(const std::string &_cyphertext_b64, const std::string &_key, std::string &_plaintext)
  {
      std::vector<uint8_t> message_cyphertext;
      Base64::Decode(_cyphertext_b64, message_cyphertext);
      std::vector<uint8_t> message_key(_key.begin(), _key.end());

      std::vector<uint8_t> message_plaintext;
      decryptWithSymmetricCiphers(message_cyphertext, message_key, message_plaintext);
      _plaintext.clear();
      _plaintext.reserve(message_cyphertext.size());
      _plaintext.assign(message_plaintext.begin(), message_plaintext.end());

  }

} // namespace worlddirect
