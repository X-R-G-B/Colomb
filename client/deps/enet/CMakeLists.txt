cmake_minimum_required(VERSION 3.15)

include(FetchContent)

FetchContent_Declare(
    enet
    CMAKE_ARGS "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_CONFIGURATION_TYPES=\"Release;Release\"" "-DCONFIG=Release"
    URL "https://github.com/X-R-G-B/Colomb/releases/latest/download/enet.tar"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(enet)

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
    $<BUILD_INTERFACE:${enet_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${enet_SOURCE_DIR}/include
)

install(
    DIRECTORY ${enet_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
