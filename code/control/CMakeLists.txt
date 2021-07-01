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


add_subdirectory(../core core-lib)
init_control("../core" "../modules")

set(TS_FILES control_en_150.ts)


add_executable(control
    main.cpp
    test.h
    info_blocks.h
    main_window.cpp
    main_window.h
    main_window.ui

    ConnectionSetupDialog.cpp
    ConnectionSetupDialog.h
    ConnectionSetupDialog.ui
    ${TS_FILES}
    ${CORE_SOURCES})


target_link_libraries(control PRIVATE ${CORE_DEPENDENCIES})
qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})


