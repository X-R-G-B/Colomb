cmake_minimum_required(VERSION 3.15)

include(FetchContent)

FetchContent_Declare(
    httplib
    CMAKE_ARGS "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_CONFIGURATION_TYPES=\"Release;Release\"" "-DCONFIG=Release"
    URL "https://github.com/X-R-G-B/Colomb/releases/latest/download/httplib.tar"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(httplib)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    ${HTTPLIB_LIBRARY}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${HTTPLIB_INCLUDE_DIR}
)
