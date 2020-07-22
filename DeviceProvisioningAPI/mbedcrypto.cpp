#include "mbedcrypto.h"

#include "psa/crypto.h"
#include "psa/crypto_extra.h"
#include "mbedtls/version.h"

#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <sstream>


#include "base64.h"

namespace worlddirect {

  constexpr size_t tag_length = 8;
  constexpr auto alg = PSA_ALG_AEAD_WITH_TAG_LENGTH(PSA_ALG_CCM, tag_length);

  MbedCrypto::MbedCrypto()
  {

  }

  void MbedCrypto::encryptWithSymmetricCiphers(const std::vector<uint8_t> &_plaintext, const std::vector<uint8_t> &_key, std::vector<uint8_t> &_cyphertext)
  {
    resetAllErrors();
    if(_plaintext.empty()){
        m_lastErrorMessage = "plaintext is empty";
        return;
      }

    if(_key.empty()){
        m_lastErrorMessage = "key is empty";
        return;
      }

    auto key = _key.data();
    auto key_len = _key.size();

    constexpr size_t nonce_len = 12;
    uint8_t nonce[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};

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
    m_status = psa_crypto_init();
    if (m_status != PSA_SUCCESS) {
        setPsaError("Failed to initialize PSA Crypto");
        return;
      }

    output_size = input_data_len + tag_length;
    output_data = (uint8_t *)malloc(output_size);
    if (!output_data) {
        m_lastErrorMessage = "Out of memory";
        return;
      }

    /* Import a key */
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attributes, 128);
    m_status = psa_import_key(&attributes, key, key_len, &handle);
    if (m_status != PSA_SUCCESS) {
        setPsaError("Failed to import a key");
        return;
      }
    psa_reset_key_attributes(&attributes);

    /* Authenticate and encrypt */
    m_status = psa_aead_encrypt(handle, alg,
                              nonce, nonce_len,
                              additional_data, additional_data_len,
                              input_data, input_data_len,
                              output_data, output_size,
                              &output_length);
    if (m_status != PSA_SUCCESS) {
        setPsaError("Failed to authenticate and encrypt");
        return;
      }
    _cyphertext.clear();
    _cyphertext.reserve(nonce_len + output_length);
    _cyphertext.assign(nonce, nonce+nonce_len);
    _cyphertext.insert(_cyphertext.end(), output_data, output_data+output_length);

    /* Clean up */
    free(output_data);

    /* Destroy the key */
    psa_destroy_key(handle);

    mbedtls_psa_crypto_free();
  }

  void worlddirect::MbedCrypto::decryptWithSymmetricCiphers(const std::vector<uint8_t> &_cyphertext, const std::vector<uint8_t> &_key,  std::vector<uint8_t> &_plaintext)
  {
    resetAllErrors();
    if(_cyphertext.empty()){
        m_lastErrorMessage = "cyphertext is empty";
        return;
      }

    if(_key.empty()){
        m_lastErrorMessage = "key is empty";
        return;
      }

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
    m_status = psa_crypto_init();
    if (m_status != PSA_SUCCESS) {
        setPsaError("Failed to initialize PSA Crypto");
        return;
      }

    output_size = input_data_len;
    output_data = (uint8_t *)malloc(output_size);
    if (!output_data) {
        m_lastErrorMessage = "Out of memory";
        return;
      }

    /* Import a key */
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attributes, 128);
    m_status = psa_import_key(&attributes, key, key_len, &handle);
    if (m_status != PSA_SUCCESS) {
        setPsaError("Failed to import a key");
        return;
      }
    psa_reset_key_attributes(&attributes);

    m_status = psa_aead_decrypt(handle, alg,
                              nonce, nonce_len,
                              additional_data, additional_data_len,
                              input_data, input_data_len,
                              output_data, output_size,
                              &output_length);
    if (m_status != PSA_SUCCESS) {
        setPsaError("Failed to authenticate and decrypt");
        return;
      }

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

  bool MbedCrypto::hadError() const
  {
    return (m_status != PSA_SUCCESS) || (m_lastErrorMessage.empty() == false);
  }

  std::string MbedCrypto::errorMessage() const
  {
    return m_lastErrorMessage;
  }

  void MbedCrypto::resetAllErrors()
  {
    m_status = PSA_SUCCESS;
    m_lastErrorMessage.clear();
  }

  void MbedCrypto::setPsaError(std::string msg)
  {
    std::stringstream stringBuilder;
    stringBuilder << "PSA error " << msg <<" "<<psacode2status(m_status);

    m_lastErrorMessage = stringBuilder.str();
  }

  std::string MbedCrypto::psacode2status(psa_status_t code)
  {
    if(code == PSA_SUCCESS){
        return "The action was completed successfully.";
      }

    if(code == PSA_ERROR_NOT_SUPPORTED){
        return "The requested operation or a parameter is not supported by this implementation.";
      }

    if(code == PSA_ERROR_NOT_PERMITTED){
        return "The requested action is denied by a policy.";
      }

    if(code == PSA_ERROR_BUFFER_TOO_SMALL){
        return "An output buffer is too small.";
      }

    if(code == PSA_ERROR_ALREADY_EXISTS){
        return "Asking for an item that already exists";
      }

    if(code == PSA_ERROR_DOES_NOT_EXIST){
        return "Asking for an item that doesn't exist";
      }

    if(code == PSA_ERROR_BAD_STATE){
        return "The requested action cannot be performed in the current state.";
      }

    if(code == PSA_ERROR_INVALID_ARGUMENT){
        return "The parameters passed to the function are invalid.";
      }

    if(code == PSA_ERROR_INSUFFICIENT_MEMORY){
        return "There is not enough runtime memory.";
      }

    if(code == PSA_ERROR_INSUFFICIENT_STORAGE){
        return "There is not enough persistent storage.";
      }

    if(code == PSA_ERROR_COMMUNICATION_FAILURE){
        return "There was a communication failure inside the implementation.";
      }

    if(code == PSA_ERROR_STORAGE_FAILURE){
        return "There was a storage failure that may have led to data loss.";
      }

    if(code == PSA_ERROR_HARDWARE_FAILURE){
        return "A hardware failure was detected.";
      }

    if(code == PSA_ERROR_CORRUPTION_DETECTED){
        return "A tampering attempt was detected.";
      }

    if(code == PSA_ERROR_INSUFFICIENT_ENTROPY){
        return "There is not enough entropy to generate random data needed for the requested action.";
      }

    if(code == PSA_ERROR_INVALID_SIGNATURE){
        return "The signature, MAC or hash is incorrect.";
      }

    if(code == PSA_ERROR_INVALID_PADDING){
        return "The decrypted padding is incorrect.";
      }

    if(code == PSA_ERROR_INSUFFICIENT_DATA){
        return "Return this error when there's insufficient data when attempting to read from a resource.";
      }

    if(code == PSA_ERROR_INVALID_HANDLE){
        return "The key handle is not valid.";
      }

    return "An error occurred that does not correspond to any defined failure cause.";
  }

} // namespace worlddirect
