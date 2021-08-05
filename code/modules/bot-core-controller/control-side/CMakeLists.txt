cmake_minimum_required(VERSION 3.16)

project(bot-core-controller LANGUAGES CXX)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(BOT_CORE_CONTROLLER_MODULE_SOURCES
        BotCoreController.cpp
        BotCoreController.h

        ModuleDialogue.cpp
        ModuleDialogue.h
        ModuleDialogue.ui

        ../../Module.cpp
        ../../GlobalResources.cpp
        ../../Module.h
        ../../UIModule.h)

add_library(bot-core-controller SHARED ${BOT_CORE_CONTROLLER_MODULE_SOURCES})
add_compile_definitions(MODULE_CLASS=BotCoreController MODULE_FOLDER=bot-core-controller/control-side)

target_link_libraries(bot-core-controller Qt5::Widgets)
