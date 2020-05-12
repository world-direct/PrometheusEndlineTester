cmake_minimum_required(VERSION 3.5)

project(PrometheusEndlineTester
    VERSION 0.1.0
    LANGUAGES C CXX ASM
    )

set(SETTINGS_FILE_NAME ${PROJECT_NAME})
string(APPEND SETTINGS_FILE_NAME ".ini")

configure_file(config.h.in config.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include("common_compile_flags.cmake")

#########################################
## SubProjects
add_subdirectory(PrometheusEndlineTesterLib)
#########################################

#########################################
## SubProjects
add_subdirectory(PrometheusEndlineTester)
#########################################
