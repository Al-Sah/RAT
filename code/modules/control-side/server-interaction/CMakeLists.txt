cmake_minimum_required(VERSION 3.16)
project(server_interaction)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(MODULE_SOURCES 
	serverinteractiongroupbox.h
	serverinteractiongroupbox.cpp
        serverinteractiongroupbox.ui
        testmainwindow.cpp
        testmainwindow.ui
        testmainwindow.h
        serverInteraction.cpp
        ServerInteraction.h
        ../../module.cpp
        ../../Module.h
        ../../module_resources.cpp
        ../Module.h)

add_compile_definitions(MODULE_CLASS=ServerInteraction MODULE_FOLDER=control-side/server-interaction)

add_library(server_interaction SHARED ${MODULE_SOURCES})
#add_executable(server_interaction_ ${MODULE_SOURCES} main.cpp)

target_link_libraries (server_interaction  Qt5::Widgets)
#target_link_libraries (server_interaction_  Qt5::Widgets)
