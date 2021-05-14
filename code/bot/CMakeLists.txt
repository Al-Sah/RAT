cmake_minimum_required(VERSION 3.19)
project(bot)

set(CMAKE_CXX_STANDARD 20)

option(BOT_ENABLE ON)
include_directories(../core ../core/websocket-lib ../modules)
add_subdirectory(../core core-lib)

set(CORE_SOURCES
        ../core/impl/WebsocketRunner.cpp
        ../core/impl/CommandsManager.cpp
        ../core/impl/System.cpp
        ../core/impl/ApplicationContext.cpp
        ../core/impl/ModulesManager.cpp
        ../core/models/src/ParsedTextMessage.cpp
        ../core/models/src/TaskResult.cpp
        ../core/models/src/Task.cpp)

add_executable(bot ${CORE_SOURCES} main.cpp)

#add_subdirectory("../modules") # FIXME

find_package(Boost COMPONENTS thread filesystem)
target_link_libraries (bot ${Boost_LIBRARIES} dl)