cmake_minimum_required(VERSION 3.16)

project(control)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TODO check manual; use "set" or "option" ?
set(BOT_ENABLE OFF)     # func "set" is used instead of option(BOT_ENABLE ON)
set(CONTROL_ENABLE ON)  # func "set" is used instead of option(CONTROL_ENABLE OFF)


add_subdirectory(../abstract-core core-lib)
init_control("../abstract-core" "../modules")

set(TS_FILES gui/control_en_150.ts)


add_executable(control
        main.cpp
        ${TS_FILES}
        ${CORE_SOURCES}

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

        ../modules/UIModule.h
        ../modules/Module.h)

target_link_libraries(control ${CORE_DEPENDENCIES})
qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})