cmake_minimum_required(VERSION 3.5)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Widgets SerialPort REQUIRED)

configure_file(${SETTINGS_FILE_NAME} ${SETTINGS_FILE_NAME} COPYONLY)

add_executable(PrometheusEndlineTester
main.cpp
ui/mainwindow.cpp
)

target_include_directories(PrometheusEndlineTester BEFORE PUBLIC ../PrometheusEndlineTesterLib)

target_link_libraries(PrometheusEndlineTester PRIVATE Qt5::Widgets Qt5::SerialPort PrometheusEndlineTesterLib)
