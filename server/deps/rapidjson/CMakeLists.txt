cmake_minimum_required(VERSION 3.15)

include(FetchContent)

FetchContent_Declare(
    rapidjson
    CMAKE_ARGS "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_CONFIGURATION_TYPES=\"Release;Release\"" "-DCONFIG=Release"
    URL "https://github.com/X-R-G-B/Colomb/releases/latest/download/rapidjson.tar"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(rapidjson)

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
    $<BUILD_INTERFACE:${rapidjson_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${rapidjson_SOURCE_DIR}/include
)

install(
    DIRECTORY ${rapidjson_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
