#ifndef PROMETHEUSENDLINETESTER_GLOBAL_H
#define PROMETHEUSENDLINETESTER_GLOBAL_H

#include "config.h"

constexpr const char* KEY_FLASHER_BASEPATH = "flasher/basePath";
constexpr const char* KEY_FLASHER_PATHTOCMD = "flasher/pathToCmd";
constexpr const char* KEY_FLASHER_PATHTODOCS = "flasher/pathToDocs";
constexpr const char* KEY_FLASHER_WRITEARGS = "flasher/writeArgs";
constexpr const char* KEY_FLASHER_ERASEARGS = "flasher/eraseArgs";
constexpr const char* KEY_FLASHER_RESETARGS = "flasher/resetArgs";
constexpr const char* KEY_FLASHER_CONNECTARGS = "flasher/connectArgs";
constexpr const char* KEY_FLASHER_PATHTOTEST = "flasher/pathToTestBinary";
constexpr const char* KEY_FLASHER_PATHTOFW = "flasher/pathToBinary";

constexpr const char* KEY_SERIAL_PORT = "serial/port";
constexpr const char* KEY_SERIAL_BAUD = "serial/baud";
constexpr const char* KEY_SERIAL_DATA = "serial/data";
constexpr const char* KEY_SERIAL_STOP = "serial/stop";
constexpr const char* KEY_SERIAL_PARITY = "serial/parity";
constexpr const char* KEY_SERIAL_FLOWCTRL = "serial/flowctrl";
constexpr const char* KEY_SERIAL_SYNC_TIMEOUT = "serial/sync_timeout";

constexpr const char* KEY_IDENTITY_SERVER = "identity/server";
constexpr const char* KEY_IDENTITY_CLIENT = "identity/client";
constexpr const char* KEY_IDENTITY_SECRET = "identity/secret";
constexpr const char* KEY_IDENTITY_SCOPE = "identity/scope";

constexpr const char* KEY_PROVISIONING_SERVER = "provisioning/server";
constexpr const char* KEY_PROVISIONING_NAME = "provisioning/name";

constexpr const char* KEY_UI_GEOMETRY = "ui/geometry";
constexpr const char* KEY_UI_WINDOWSTATE = "ui/windowstate";

constexpr const char* GREENTEA_TEST_ENV_END = "end";
constexpr const char* GREENTEA_TEST_ENV_EXIT = "__exit";
constexpr const char* GREENTEA_TEST_ENV_SYNC = "__sync";
constexpr const char* GREENTEA_TEST_ENV_TIMEOUT = "__timeout";
constexpr const char* GREENTEA_TEST_ENV_HOST_TEST_NAME = "__host_test_name";
constexpr const char* GREENTEA_TEST_ENV_HOST_TEST_VERSION = "__version";
constexpr const char* GREENTEA_TEST_ENV_TESTCASE_NAME = "__testcase_name";
constexpr const char* GREENTEA_TEST_ENV_TESTCASE_COUNT = "__testcase_count";
constexpr const char* GREENTEA_TEST_ENV_TESTCASE_START = "__testcase_start";
constexpr const char* GREENTEA_TEST_ENV_TESTCASE_FINISH = "__testcase_finish";
constexpr const char* GREENTEA_TEST_ENV_TESTCASE_SUMMARY = "__testcase_summary";
constexpr const char* GREENTEA_TEST_ENV_LCOV_START = "__coverage_start";

constexpr const char* GREENTEA_TEST_ENV_SUCCESS = "success";
constexpr const char* GREENTEA_TEST_ENV_FAILURE = "failure";

constexpr const char* GREENTEA_SETUP ="mbedmbedmbedmbedmbedmbedmbedmbed";

constexpr const char* GREENTEA_KV_PREAMBLE ="{{";
constexpr const char* GREENTEA_KV_SEPERATOR = ";";
constexpr const char* GREENTEA_KV_POSTAMBLE = "}}";

constexpr const char* GREENTEA_MSG_PREAMBLE =">>>";

constexpr const char* GREENTEA_MSG_ENDL ="\r\n";

constexpr const char* CO1_FUNCTIONAL_ENDLINE_TEST_TYPE= "__type";
constexpr const char* CO1_FUNCTIONAL_ENDLINE_TEST_HWVER= "__hw_ver";
constexpr const char* CO1_FUNCTIONAL_ENDLINE_TEST_EPNAME= "__ep_name";
constexpr const char* CO1_FUNCTIONAL_ENDLINE_TEST_ICCID= "__icc_id";

#endif // PROMETHEUSENDLINETESTER_GLOBAL_H
