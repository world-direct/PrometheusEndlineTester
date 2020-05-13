#ifndef PROMETHEUSENDLINETESTER_GLOBAL_H
#define PROMETHEUSENDLINETESTER_GLOBAL_H

#include "config.h"

constexpr const char* KEY_FLASHER_PATHTOCMD = "flasher/pathToCmd";
constexpr const char* KEY_FLASHER_WRITEARGS = "flasher/writeArgs";
constexpr const char* KEY_FLASHER_ERASEARGS = "flasher/eraseArgs";
constexpr const char* KEY_FLASHER_RESETARGS = "flasher/resetArgs";
constexpr const char* KEY_FLASHER_PATHTOFW = "flasher/pathToBinary";

constexpr const char* KEY_SERIAL_PORT = "serial/port";
constexpr const char* KEY_SERIAL_BAUD = "serial/baud";
constexpr const char* KEY_SERIAL_DATA = "serial/data";
constexpr const char* KEY_SERIAL_STOP = "serial/stop";
constexpr const char* KEY_SERIAL_PARITY = "serial/parity";
constexpr const char* KEY_SERIAL_FLOWCTRL = "serial/flowctrl";

constexpr const char* KEY_UI_GEOMETRY = "ui/geometry";
constexpr const char* KEY_UI_WINDOWSTATE = "ui/windowstate";


#endif // PROMETHEUSENDLINETESTER_GLOBAL_H
