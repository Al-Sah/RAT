cmake_minimum_required(VERSION 3.16)

project(echo LANGUAGES CXX)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(ECHO_MODULE_SOURCES
    echo.cpp
    echodialogue.cpp
    echodialogue.h
    echodialogue.ui

    resources.h
    Echo.h
    ../../module.cpp
    ../../module_resources.cpp
    ../../Module.h
    ../Module.h)

set(ECHO_MODULE_SOURCES_TEST
    ${ECHO_MODULE_SOURCES}
    main.cpp
    testmainwindow.cpp
    testmainwindow.h
    testmainwindow.ui)


add_library(echo SHARED ${ECHO_MODULE_SOURCES})
add_executable(echo_test ${ECHO_MODULE_SOURCES_TEST})
add_compile_definitions(MODULE_CLASS=Echo MODULE_FOLDER=control-side/echo)

target_link_libraries(echo Qt5::Widgets)
target_link_libraries(echo_test Qt5::Widgets)

#target_compile_definitions(echo PRIVATE ECHO_LIBRARY)
