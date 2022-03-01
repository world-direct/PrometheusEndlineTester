cmake_minimum_required(VERSION 3.5)

set(TARGET_SETTINGS_FILE_NAME "SerialEchoServer.ini")
#string(APPEND TARGET_SETTINGS_FILE_NAME ".ini")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# find_package(Qt5 COMPONENTS Widgets SerialPort REQUIRED)
# find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5 COMPONENTS SerialPort REQUIRED)

configure_file(${TARGET_SETTINGS_FILE_NAME} ${TARGET_SETTINGS_FILE_NAME} COPYONLY)

add_executable(SerialEchoServer
    WIN32
    main.cpp
    clientthread.cpp
    )

target_link_libraries(SerialEchoServer PRIVATE Qt5::SerialPort)

target_compile_definitions(SerialEchoServer PRIVATE TARGET_SETTINGS_FILE_NAME=${TARGET_SETTINGS_FILE_NAME})
