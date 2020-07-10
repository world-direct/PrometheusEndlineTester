cmake_minimum_required(VERSION 3.5)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Widgets SerialPort REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

#-------------------------------
if(Qt5_FOUND AND WIN32 AND TARGET Qt5::qmake AND NOT TARGET Qt5::windeployqt)
    get_target_property(_qt5_qmake_location Qt5::qmake IMPORTED_LOCATION)

    execute_process(
        COMMAND "${_qt5_qmake_location}" -query QT_INSTALL_PREFIX
        RESULT_VARIABLE return_code
        OUTPUT_VARIABLE qt5_install_prefix
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(imported_location "${qt5_install_prefix}/bin/windeployqt.exe")

    if(EXISTS ${imported_location})
        add_executable(Qt5::windeployqt IMPORTED)

        set_target_properties(Qt5::windeployqt PROPERTIES
            IMPORTED_LOCATION ${imported_location}
        )
    endif()
endif()
#-------------------------------

configure_file(${SETTINGS_FILE_NAME} ${SETTINGS_FILE_NAME} COPYONLY)
configure_file(${STYLESHEET_FILE_NAME} ${STYLESHEET_FILE_NAME} COPYONLY)
configure_file(${SEED_FILE_NAME} ${SEED_FILE_NAME} COPYONLY)

add_executable(PrometheusEndlineTester
    WIN32
    main.cpp
    settings/settings.cpp
    stlink/stlinkdeviceflasher.cpp
    htrun/mbedhosttestrunner.cpp
    statemachine/statemachine.cpp
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
    provisioning/deviceprovisioning.cpp
    )

target_link_libraries(PrometheusEndlineTester PRIVATE Qt5::PrintSupport Qt5::Widgets Qt5::SerialPort QRCodeGenerator DeviceProvisioningAPIClent )

message($<TARGET_FILE_DIR:PrometheusEndlineTester>/$<TARGET_FILE_NAME:PrometheusEndlineTester>)

#-------------------------------
if(TARGET Qt5::windeployqt)
    # execute windeployqt in a tmp directory after build
    add_custom_command(TARGET PrometheusEndlineTester
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/windeployqt"
        COMMAND set PATH=%PATH%$<SEMICOLON>${qt5_install_prefix}/bin
        COMMAND Qt5::windeployqt --dir "${CMAKE_CURRENT_BINARY_DIR}/windeployqt" "$<TARGET_FILE_DIR:PrometheusEndlineTester>/$<TARGET_FILE_NAME:PrometheusEndlineTester>"
        WORKING_DIRECTORY $<TARGET_FILE_DIR:PrometheusEndlineTester>
    )

    # copy deployment directory during installation
    # install(
    #     DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/windeployqt/"
    #    DESTINATION ${PROMETHEUSENDLINETESTER_INSTALL_RUNTIME_DESTINATION}
    # )
endif()
#-------------------------------
