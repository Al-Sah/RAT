cmake_minimum_required(VERSION 3.16)

project(control)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()


option(BOT_ENABLE ON)

option(CONTROL_ENABLE ON)


include_directories(../core ../core/websocket-lib ../modules)
add_subdirectory(../core core-lib)

set(CORE_SOURCES
        ../core/impl/WebsocketRunner.cpp
        ../core/impl/CommandsManager.cpp
        ../core/impl/System.cpp
        ../core/impl/ApplicationContext.cpp
        ../core/impl/ModulesManager.cpp
        ../core/impl/DataTransfer.cpp
        ../core/models/src/ParsedTextMessage.cpp
        ../core/models/src/TaskResult.cpp
        ../core/models/src/TaskInfo.cpp)

set(CORE_HEADERS  ../core/DataTransfer.h)

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
#find_package(Qt<QTVERSION> COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Boost COMPONENTS thread filesystem REQUIRED)

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
    ${CORE_SOURCES}
    ${CORE_HEADERS}
)


set(MODULE_SOURCES
        ../modules/control-side/server-interaction/serverinteractiongroupbox.h
        ../modules/control-side/server-interaction/serverinteractiongroupbox.cpp
        ../modules/control-side/server-interaction/serverinteractiongroupbox.ui
        ../modules/control-side/server-interaction/testmainwindow.cpp
        ../modules/control-side/server-interaction/testmainwindow.ui
        ../modules/control-side/server-interaction/testmainwindow.h
        ../modules/control-side/server-interaction/serverInteraction.cpp
        ../modules/control-side/server-interaction/ServerInteraction.h
        ../modules/control-side/Module.h
        ../modules/module.cpp
        ../modules/module_resources.cpp)

add_compile_definitions(MODULE_CLASS=ServerInteraction MODULE_FOLDER=../modules/control-side/server-interaction)

add_library(server_interaction SHARED ${MODULE_SOURCES})
target_link_libraries (server_interaction Qt5::Widgets)

#[[

set(ECHO_MODULE_SOURCES
    ../modules/control-side/echo/echo.cpp
    ../modules/control-side/echo/echodialogue.cpp
    ../modules/control-side/echo/echodialogue.h
    ../modules/control-side/echo/echodialogue.ui
    ../modules/control-side/echo/resources.h
    ../modules/control-side/echo/Echo.h
    ../modules/control-side/Module.h
    ../modules/module.cpp)

add_library(echo SHARED ${ECHO_MODULE_SOURCES})
add_compile_definitions(MODULE_CLASS=Echo MODULE_FOLDER=../modules/control-side/echo)

target_link_libraries(echo Qt5::Widgets)
]]


target_link_libraries(control PRIVATE Qt5::Widgets ${Boost_LIBRARIES} dl)
qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
