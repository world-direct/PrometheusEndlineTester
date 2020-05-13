cmake_minimum_required(VERSION 3.5)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core SerialPort REQUIRED)


add_library(PrometheusEndlineTesterLib SHARED
  PrometheusEndlineTesterLib_global.h
  stlinkdeviceflasher.cpp
  stlinkdeviceflasher.h
  mbedhosttestrunner.cpp
  mbedhosttestrunner.h
)

target_link_libraries(PrometheusEndlineTesterLib PRIVATE Qt5::Core Qt5::SerialPort)

target_compile_definitions(PrometheusEndlineTesterLib PRIVATE PROMETHEUSENDLINETESTERLIB_LIBRARY)
