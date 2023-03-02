cmake_minimum_required(VERSION 3.5)

find_package(OpenSSL REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core)
qt_standard_project_setup()

set(POSSUM_SOURCES
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        session.cpp session.h image.cpp image.h)
add_library(possum_core ${POSSUM_SOURCES})

SET(CMAKE_BUILD_TYPE Debug)

add_executable(picture_possum
        main.cpp)

target_link_libraries(possum_core PRIVATE Qt6::Widgets OpenSSL::SSL)
target_link_libraries(picture_possum PRIVATE Qt6::Widgets OpenSSL::SSL)
target_link_libraries(picture_possum PUBLIC possum_core)
set_target_properties(picture_possum PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
