cmake_minimum_required(VERSION 3.16)
project(core)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost COMPONENTS thread filesystem)

include_directories(../websocket-lib/)

set(CORE_SOURCES
        main.cpp
        impl/WebsocketRunner.cpp
        impl/MessagesManager.cpp
        impl/System.cpp
        impl/ApplicationContext.cpp
        impl/ModulesManager.cpp
        impl/ParsedTextMessage.cpp)

add_executable(core ${CORE_SOURCES})

add_subdirectory("modules")

target_link_libraries (core ${Boost_LIBRARIES} dl)