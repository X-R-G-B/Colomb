cmake_minimum_required(VERSION 3.15)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp
)

add_subdirectory(utils)
add_subdirectory(menu)
add_subdirectory(uiconf)
