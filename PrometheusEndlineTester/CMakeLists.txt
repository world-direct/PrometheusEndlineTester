cmake_minimum_required(VERSION 3.5)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Widgets SerialPort REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

configure_file(${SETTINGS_FILE_NAME} ${SETTINGS_FILE_NAME} COPYONLY)
configure_file(${STYLESHEET_FILE_NAME} ${STYLESHEET_FILE_NAME} COPYONLY)

add_executable(PrometheusEndlineTester
    WIN32
    main.cpp
    settings/settings.cpp
    stlink/stlinkdeviceflasher.cpp
    htrun/mbedhosttestrunner.cpp
    statemachine/statemachine.cpp
    nameplate/nameplate.cpp
    ui/widgets/testexplorer.cpp
    ui/mainwindow.cpp
    ui/dialogs/settingsdialog.cpp
    ui/widgets/console.cpp
    ui/widgets/qrwidget.cpp
    ui/widgets/nameplatewidget.cpp
    PrometheusEndlineTester.qrc
    ui/model/testcasesumarymodel.cpp
    ui/model/hosttestrun.cpp
    ui/model/hosttest.cpp
    ui/model/testcase.cpp
    ui/widgets/dcplatewidget.cpp
    )


target_link_libraries(PrometheusEndlineTester PRIVATE Qt5::PrintSupport Qt5::Widgets Qt5::SerialPort QRCodeGenerator)
