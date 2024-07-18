cmake_minimum_required(VERSION 3.13)
project(picture_possum VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
#list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
#include(Infrastructure)

find_package(OpenSSL)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core)
qt_standard_project_setup()
add_subdirectory(src)
add_subdirectory(test)

