cmake_minimum_required(VERSION 3.5)

#qt_standard_project_setup()

set(POSSUM_SOURCES
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        image.cpp
        image.h
        imagesListModel.cpp
        imagesListModel.h
        TinyEXIF.cpp
        TinyEXIF.h
        Settings.cpp
        Settings.h
        Tag.cpp
        Tag.h
        ImageTypes.h
        ImageTypes.cpp
        settingswidget.cpp
        TagListModel.cpp
        TagListModel.h
        tagdialog.cpp
        tagdialog.h
        )
add_library(possum_core ${POSSUM_SOURCES})

SET(CMAKE_BUILD_TYPE Debug)

add_executable(picture_possum
        main.cpp)

target_link_libraries(possum_core PUBLIC Qt6::Widgets OpenSSL::SSL)
target_link_libraries(picture_possum PUBLIC Qt6::Widgets OpenSSL::SSL)
target_link_libraries(picture_possum PUBLIC possum_core)
set_target_properties(picture_possum PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS picture_possum DESTINATION bin)
