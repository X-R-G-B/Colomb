cmake_minimum_required(VERSION 3.15)

include(FetchContent)

FetchContent_Declare(
    httplib
    CMAKE_ARGS "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_CONFIGURATION_TYPES=\"Release;Release\"" "-DCONFIG=Release" "-DHTTPLIB_COMPILE=ON"
    URL "https://github.com/X-R-G-B/Colomb/releases/latest/download/httplib.tar"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(httplib)

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
    $<BUILD_INTERFACE:${httplib_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${httplib_SOURCE_DIR}/include
)

install(
    DIRECTORY ${httplib_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
