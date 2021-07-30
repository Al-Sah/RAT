cmake_minimum_required(VERSION 3.16)

project(control VERSION 1.0.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Getting abstract core sources and dependencies
add_subdirectory(../abstract-core rat-core)
init_core("../abstract-core" "../modules")

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
set(TS_FILES control_en_150.ts)

add_executable(control
        main.cpp
        ${TS_FILES}
        ${ABSTRACT_CORE_SOURCES}

        gui/info_blocks.h
        gui/main_window.cpp
        gui/main_window.h
        gui/main_window.ui

        gui/ConnectionSetupDialog.cpp
        gui/ConnectionSetupDialog.h
        gui/ConnectionSetupDialog.ui

        core-impl/DataTransfer.h
        core-impl/impl/Control.cpp
        core-impl/impl/DataTransfer.cpp
        core-impl/impl/ApplicationContext.cpp
        core-impl/impl/TaskManager.cpp
        core-impl/impl/WebSocketRunner.cpp
        core-impl/impl/ModulesManager.cpp

        ../modules/UIModule.h)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
target_link_libraries(control ${ABSTRACT_CORE_DEPENDENCIES} Qt5::Widgets )

# [OPTIONAL] Enable building external control modules
set(BUILD_BOT_MODULES OFF)
set(BUILD_CONTROL_MODULES ON)
add_subdirectory(../modules modules)