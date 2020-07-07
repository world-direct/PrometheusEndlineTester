cmake_minimum_required(VERSION 3.5)

project(PrometheusEndlineTester
    VERSION 0.1.0
    LANGUAGES C CXX ASM
    )

set(SETTINGS_FILE_NAME ${PROJECT_NAME})
string(APPEND SETTINGS_FILE_NAME ".ini")

set(STYLESHEET_FILE_NAME ${PROJECT_NAME})
string(APPEND STYLESHEET_FILE_NAME ".qss")

configure_file(config.h.in config.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include("common_compile_flags.cmake")

#########################################
## QR-Code-generator
add_library(QRCodeGenerator
    QR-Code-generator/cpp/QrCode.cpp
    )

target_include_directories(QRCodeGenerator
    SYSTEM
    PUBLIC QR-Code-generator/cpp
    )
#########################################

include("strict_compile_flags.cmake")

#########################################
## DeviceProvisioningAPI
add_subdirectory(DeviceProvisioningAPI)
#########################################


#########################################
## PrometheusEndlineTester
add_subdirectory(PrometheusEndlineTester)
#########################################
