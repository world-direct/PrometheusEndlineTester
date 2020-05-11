cmake_minimum_required(VERSION 3.5)

project(PrometheusEndlineTesterLib LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core LinguistTools REQUIRED)


add_library(PrometheusEndlineTesterLib SHARED
  PrometheusEndlineTesterLib_global.h
  prometheusendlinetesterlib.cpp
  prometheusendlinetesterlib.h
)

target_link_libraries(PrometheusEndlineTesterLib PRIVATE Qt5::Core)

target_compile_definitions(PrometheusEndlineTesterLib PRIVATE PROMETHEUSENDLINETESTERLIB_LIBRARY)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
