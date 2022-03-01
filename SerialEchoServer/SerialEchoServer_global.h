#ifndef SERIALECHOSERVER_GLOBAL_H
#define SERIALECHOSERVER_GLOBAL_H

#include "config.h"

// If you want to stringify the result of expansion of a macro argument, you have to use two levels of macros.
//#define xstr(s) str(s)
//#define str(s) #s

constexpr const char* SES_SETT_FILE_NAME = "SerialEchoServer.ini";

constexpr const char* KEY_SERIAL_PORT = "serial/port";
constexpr const char* KEY_SERIAL_BAUD = "serial/baud";
constexpr const char* KEY_SERIAL_DATA = "serial/data";
constexpr const char* KEY_SERIAL_STOP = "serial/stop";
constexpr const char* KEY_SERIAL_PARITY = "serial/parity";
constexpr const char* KEY_SERIAL_FLOWCTRL = "serial/flowctrl";
constexpr const char* KEY_SERIAL_WAIT_TIMEOUT = "serial/wait_timeout";

#endif // SERIALECHOSERVER_GLOBAL_H
