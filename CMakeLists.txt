cmake_minimum_required(VERSION 3.5)

project(picture_possum VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        session.cpp session.h image.cpp image.h)
SET(CMAKE_BUILD_TYPE Debug)

add_executable(picture_possum
        ${PROJECT_SOURCES})

target_link_libraries(picture_possum PRIVATE Qt6::Widgets OpenSSL::SSL)

set_target_properties(picture_possum PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
