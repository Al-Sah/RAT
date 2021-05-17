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
        ../core/models/src/Task.cpp)

set(CORE_HEADERS  ../core/DataTransfer.h)

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
#find_package(Qt<QTVERSION> COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Boost COMPONENTS thread filesystem REQUIRED)

set(TS_FILES control_en_150.ts)


add_library(zzz SHARED test_2.cpp)
target_link_libraries(zzz PRIVATE Qt5::Widgets)


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


target_link_libraries(control PRIVATE Qt5::Widgets ${Boost_LIBRARIES} dl zzz)
qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
