cmake_minimum_required(VERSION 3.16)
project(core)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost COMPONENTS thread filesystem)

#target_include_directories(core /websocket-lib)
#include_directories(websocket-lib ../modules)
#target_include_directories(core websocket-lib)

set(CORE_EXTERNAL_LIBS ${Boost_LIBRARIES} dl)

set(CORE_SOURCES
        impl/WebsocketRunner.cpp
        impl/CommandsManager.cpp
        impl/System.cpp
        impl/ApplicationContext.cpp
        impl/ModulesManager.cpp
        models/src/ParsedTextMessage.cpp
        models/src/TaskResult.cpp
        models/src/Task.cpp)


option(HEADERS_INCLUDES_ENABLE "zz" OFF)
if(HEADERS_INCLUDES_ENABLE)
    message(STATUS "HEADERS_INCLUDES_ENABLE ON")
else()
    message(STATUS "HEADERS_INCLUDES_ENABLE OFF")
endif()


#option(BOT_ENABLE "BOT_ENABLE"              OFF)
if(BOT_ENABLE)
    message(STATUS "BOT_ENABLE              ON")
    message(STATUS "Building BOT target ")
else()
    message(STATUS "BOT_ENABLE              OFF")
endif()

#option(CONTROL_ENABLE "CONTROL_ENABLE"      OFF)
if(CONTROL_ENABLE)
    message(STATUS "CONTROL_ENABLE          ON")
else()
    message(STATUS "CONTROL_ENABLE          OFF")
endif()


if(BOT_ENABLE)
    message(STATUS "Passing CORE_SOURCES to parent BOT-SIDE project")
endif()
if(CONTROL_ENABLE)
    message(STATUS "Passing CORE_SOURCES to parent CONTROL-SIDE project")
endif()


configure_file(cmake_configuration.h.in cmake_configuration.h)

if(NOT BOT_ENABLE AND NOT CONTROL_ENABLE)
    message(STATUS "Test building")
    include_directories(websocket-lib ../modules)
    add_executable(test ${CORE_SOURCES} main.cpp)
    target_link_libraries (test ${CORE_EXTERNAL_LIBS})
endif()

#add_library(zzz1 ${CORE_SOURCES})

#add_executable(core ${CORE_SOURCES})
#add_custom_target(core ${CORE_SOURCES})
