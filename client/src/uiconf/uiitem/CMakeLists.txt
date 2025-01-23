cmake_minimum_required(VERSION 3.15)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Asset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UIButtonImage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UIButtonText.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UIDiv.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UIPopUp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UITextEntry.cpp
)
