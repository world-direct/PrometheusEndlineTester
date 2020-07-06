cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

include("C:/dev/src/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(CURL_DIR "C:/dev/src/vcpkg/packages/curl_x64-windows/share/curl")

find_package(CURL CONFIG REQUIRED)
find_package(Boost 1.60.0 REQUIRED COMPONENTS filesystem system regex)
find_package(Threads REQUIRED)

link_directories(
    ${Boost_LIBRARY_DIRS}
)

add_library(DeviceProvisioningAPIClent
    STATIC
    deviceprovisioningapiclient.cpp
    identityclient.cpp
    token.cpp
)

target_include_directories(DeviceProvisioningAPIClent
    SYSTEM PUBLIC
    ./
    DeviceProvisioningAPI/
    ${Boost_INCLUDE_DIRS}
    )

target_link_libraries(DeviceProvisioningAPIClent
    CURL::libcurl
    Boost::filesystem
    Boost::system
    Boost::regex
    Threads::Threads
    ${CMAKE_DL_LIBS}
    )


