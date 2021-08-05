cmake_minimum_required(VERSION 3.16)

project(picturesTaker LANGUAGES CXX)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(PT_MODULE_SOURCES
        PicturesTaker.cpp
        PicturesTakerDialogue.cpp
        PicturesTaker.h
        PicturesTakerDialogue.ui
        PicturesTakerDialogue.h

        resources.h
        PicturesTaker.h
        ../../Module.cpp
        ../../GlobalResources.cpp
        ../../Module.h
        ../../UIModule.h)

set(PT_MODULE_SOURCES_TEST

    ${PT_MODULE_SOURCES}
        main.cpp
        testmainwindow.cpp
        testmainwindow.h
        testmainwindow.ui)


add_library(picturesTaker SHARED ${PT_MODULE_SOURCES})
add_executable(picturesTaker_test ${PT_MODULE_SOURCES_TEST})
add_compile_definitions(MODULE_CLASS=PicturesTaker MODULE_FOLDER=picturesTaker/control-side)

target_link_libraries(picturesTaker Qt5::Widgets)
target_link_libraries(picturesTaker_test Qt5::Widgets)

#target_compile_definitions(echo PRIVATE ECHO_LIBRARY)
