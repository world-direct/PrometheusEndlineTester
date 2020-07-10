cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

include("C:/dev/src/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(CURL_DIR "C:/dev/src/vcpkg/packages/curl_x64-windows/share/curl")

find_package(CURL CONFIG REQUIRED)
find_package(Boost 1.60.0 REQUIRED COMPONENTS filesystem system regex)
find_package(Threads REQUIRED)

link_directories(
    ${Boost_LIBRARY_DIRS}
)

add_library(DeviceProvisioningAPIClent
    STATIC
    deviceprovisioningapiclient.cpp
    identityclient.cpp
    token.cpp
    mbedcrypto.cpp
    base64.cpp
    mbed_assert.c
    ../mbed-os/features/mbedtls/src/aes.c
    ../mbed-os/features/mbedtls/src/aesni.c
    ../mbed-os/features/mbedtls/src/arc4.c
    ../mbed-os/features/mbedtls/src/aria.c
    ../mbed-os/features/mbedtls/src/asn1parse.c
    ../mbed-os/features/mbedtls/src/asn1write.c
    ../mbed-os/features/mbedtls/src/base64.c
    ../mbed-os/features/mbedtls/src/bignum.c
    ../mbed-os/features/mbedtls/src/blowfish.c
    ../mbed-os/features/mbedtls/src/camellia.c
    ../mbed-os/features/mbedtls/src/ccm.c
    ../mbed-os/features/mbedtls/src/certs.c
    ../mbed-os/features/mbedtls/src/chacha20.c
    ../mbed-os/features/mbedtls/src/chachapoly.c
    ../mbed-os/features/mbedtls/src/cipher.c
    ../mbed-os/features/mbedtls/src/cipher_wrap.c
    ../mbed-os/features/mbedtls/src/cmac.c
    ../mbed-os/features/mbedtls/src/ctr_drbg.c
    ../mbed-os/features/mbedtls/src/debug.c
    ../mbed-os/features/mbedtls/src/des.c
    ../mbed-os/features/mbedtls/src/dhm.c
    ../mbed-os/features/mbedtls/src/ecdh.c
    ../mbed-os/features/mbedtls/src/ecdsa.c
    ../mbed-os/features/mbedtls/src/ecjpake.c
    ../mbed-os/features/mbedtls/src/ecp.c
    ../mbed-os/features/mbedtls/src/ecp_curves.c
    ../mbed-os/features/mbedtls/src/entropy.c
    ../mbed-os/features/mbedtls/src/entropy_poll.c
    ../mbed-os/features/mbedtls/src/error.c
    ../mbed-os/features/mbedtls/src/gcm.c
    ../mbed-os/features/mbedtls/src/havege.c
    ../mbed-os/features/mbedtls/src/hkdf.c
    ../mbed-os/features/mbedtls/src/hmac_drbg.c
    ../mbed-os/features/mbedtls/src/Makefile
    ../mbed-os/features/mbedtls/src/md.c
    ../mbed-os/features/mbedtls/src/md2.c
    ../mbed-os/features/mbedtls/src/md4.c
    ../mbed-os/features/mbedtls/src/md5.c
    ../mbed-os/features/mbedtls/src/memory_buffer_alloc.c
    ../mbed-os/features/mbedtls/src/net_sockets.c
    ../mbed-os/features/mbedtls/src/nist_kw.c
    ../mbed-os/features/mbedtls/src/oid.c
    ../mbed-os/features/mbedtls/src/padlock.c
    ../mbed-os/features/mbedtls/src/pem.c
    ../mbed-os/features/mbedtls/src/pk.c
    ../mbed-os/features/mbedtls/src/pk_wrap.c
    ../mbed-os/features/mbedtls/src/pkcs11.c
    ../mbed-os/features/mbedtls/src/pkcs12.c
    ../mbed-os/features/mbedtls/src/pkcs5.c
    ../mbed-os/features/mbedtls/src/pkparse.c
    ../mbed-os/features/mbedtls/src/pkwrite.c
    ../mbed-os/features/mbedtls/src/platform.c
    ../mbed-os/features/mbedtls/src/platform_util.c
    ../mbed-os/features/mbedtls/src/poly1305.c
    ../mbed-os/features/mbedtls/src/ripemd160.c
    ../mbed-os/features/mbedtls/src/rsa.c
    ../mbed-os/features/mbedtls/src/rsa_internal.c
    ../mbed-os/features/mbedtls/src/sha1.c
    ../mbed-os/features/mbedtls/src/sha256.c
    ../mbed-os/features/mbedtls/src/sha512.c
    ../mbed-os/features/mbedtls/src/ssl_cache.c
    ../mbed-os/features/mbedtls/src/ssl_ciphersuites.c
    ../mbed-os/features/mbedtls/src/ssl_cli.c
    ../mbed-os/features/mbedtls/src/ssl_cookie.c
    ../mbed-os/features/mbedtls/src/ssl_msg.c
    ../mbed-os/features/mbedtls/src/ssl_srv.c
    ../mbed-os/features/mbedtls/src/ssl_ticket.c
    ../mbed-os/features/mbedtls/src/ssl_tls.c
    ../mbed-os/features/mbedtls/src/threading.c
    ../mbed-os/features/mbedtls/src/timing.c
    ../mbed-os/features/mbedtls/src/version.c
    ../mbed-os/features/mbedtls/src/version_features.c
    ../mbed-os/features/mbedtls/src/x509.c
    ../mbed-os/features/mbedtls/src/x509_create.c
    ../mbed-os/features/mbedtls/src/x509_crl.c
    ../mbed-os/features/mbedtls/src/x509_crt.c
    ../mbed-os/features/mbedtls/src/x509_csr.c
    ../mbed-os/features/mbedtls/src/x509write_crt.c
    ../mbed-os/features/mbedtls/src/x509write_csr.c
    ../mbed-os/features/mbedtls/src/xtea.c
    #../mbed-os/features/mbedtls/platform/src/mbed_trng.cpp
    mbed_trng.cpp
    ../mbed-os/features/mbedtls/platform/src/platform_alt.cpp
    ../mbed-os/features/mbedtls/platform/src/shared_rng.cpp
    ../mbed-os/features/FEATURE_EXPERIMENTAL_API/FEATURE_PSA/TARGET_MBED_PSA_SRV/mbedtls/psa_crypto.c
    ../mbed-os/features/FEATURE_EXPERIMENTAL_API/FEATURE_PSA/TARGET_MBED_PSA_SRV/mbedtls/psa_crypto_slot_management.c
    # ../mbed-os/features/FEATURE_EXPERIMENTAL_API/FEATURE_PSA/src/psa_hrng.c
    psa_hrng.c
    ../mbed-os/features/FEATURE_EXPERIMENTAL_API/FEATURE_PSA/TARGET_MBED_PSA_SRV/mbedtls/psa_crypto_storage.c
    ../mbed-os/features/FEATURE_EXPERIMENTAL_API/FEATURE_PSA/TARGET_MBED_PSA_SRV/mbedtls/psa_its_file.c
)

target_include_directories(DeviceProvisioningAPIClent
    SYSTEM PUBLIC
    ./
    ../mbed-os
    ../mbed-os/hal
    ../mbed-os/platform
    ../mbed-os/platform/cxxsupport
    ../mbed-os/features/mbedtls
    ../mbed-os/features/mbedtls/inc
    ../mbed-os/features/mbedtls/inc/mbedtls
    ../mbed-os/features/mbedtls/platform/inc
    ../mbed-os/features/FEATURE_EXPERIMENTAL_API/FEATURE_PSA/inc
    ../mbed-os/features/FEATURE_EXPERIMENTAL_API/FEATURE_PSA/TARGET_MBED_PSA_SRV/inc
    ../mbed-os/features/FEATURE_EXPERIMENTAL_API/FEATURE_PSA/TARGET_MBED_PSA_SRV/inc/psa
    ../mbed-os/features/FEATURE_EXPERIMENTAL_API/FEATURE_PSA/TARGET_MBED_PSA_SRV/mbedtls
    ../mbed-os/features/FEATURE_EXPERIMENTAL_API/FEATURE_PSA/TARGET_MBED_PSA_SRV/services/storage/its
    ${Boost_INCLUDE_DIRS}
    )

add_definitions(
    -DMBEDTLS_PSA_CRYPTO_C
    -DMBEDTLS_ENTROPY_C
    -DMBEDTLS_CTR_DRBG_C
    -DMBEDTLS_ECP_C
    -DDEVICE_TRNG
    -DMBEDTLS_PSA_INJECT_ENTROPY
    -DMBEDTLS_NO_DEFAULT_ENTROPY_SOURCES
    -DMBEDTLS_ENTROPY_NV_SEED=42
    -DMBEDTLS_PSA_CRYPTO_STORAGE_C
    -DMBEDTLS_PSA_ITS_FILE_C
    -DMBEDTLS_FS_IO
    -DMBEDTLS_PLATFORM_STD_NV_SEED_FILE="./seedfile"
    )

target_link_libraries(DeviceProvisioningAPIClent
    CURL::libcurl
    Boost::filesystem
    Boost::system
    Boost::regex
    Threads::Threads
    ${CMAKE_DL_LIBS}
    )


