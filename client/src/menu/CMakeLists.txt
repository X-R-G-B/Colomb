cmake_minimum_required(VERSION 3.15)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/MenusManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MenuState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalValues.cpp
)

add_subdirectory(firstmenu)
add_subdirectory(joingamemenu)
add_subdirectory(gamemenu)
add_subdirectory(gamependingmenu)
