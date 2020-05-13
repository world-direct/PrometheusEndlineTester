#ifndef PROMETHEUSENDLINETESTERLIB_GLOBAL_H
#define PROMETHEUSENDLINETESTERLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PROMETHEUSENDLINETESTERLIB_LIBRARY)
#  define PROMETHEUSENDLINETESTERLIB_EXPORT Q_DECL_EXPORT
#else
#  define PROMETHEUSENDLINETESTERLIB_EXPORT Q_DECL_IMPORT
#endif

// these constants have been taken from mbed-os\features\frameworks\greentea-client\source\greentea_test_env.cpp
constexpr const char* GREENTEA_TEST_ENV_END = "end";
constexpr const char* GREENTEA_TEST_ENV_EXIT = "__exit";
constexpr const char* GREENTEA_TEST_ENV_SYNC = "__sync";
constexpr const char* GREENTEA_TEST_ENV_TIMEOUT = "__timeout";
constexpr const char* GREENTEA_TEST_ENV_HOST_TEST_NAME = "__host_test_name";
constexpr const char* GREENTEA_TEST_ENV_HOST_TEST_VERSION = "__version";
constexpr const char* GREENTEA_TEST_ENV_MBED = "mbedmbedmbedmbedmbedmbedmbedmbed";
constexpr const char* GREENTEA_TEST_ENV_SUCCESS = "success";
constexpr const char* GREENTEA_TEST_ENV_FAILURE = "failure";
constexpr const char* GREENTEA_TEST_ENV_TESTCASE_NAME = "__testcase_name";
constexpr const char* GREENTEA_TEST_ENV_TESTCASE_COUNT = "__testcase_count";
constexpr const char* GREENTEA_TEST_ENV_TESTCASE_START = "__testcase_start";
constexpr const char* GREENTEA_TEST_ENV_TESTCASE_FINISH = "__testcase_finish";
constexpr const char* GREENTEA_TEST_ENV_TESTCASE_SUMMARY = "__testcase_summary";
constexpr const char* GREENTEA_TEST_ENV_LCOV_START = "__coverage_start";

constexpr const char* GREENTEA_KV_PREAMBLE ="{{";
constexpr const char* GREENTEA_KV_SEPERATOR = ";";
constexpr const char* GREENTEA_KV_POSTAMBLE = "}}";


#endif // PROMETHEUSENDLINETESTERLIB_GLOBAL_H
