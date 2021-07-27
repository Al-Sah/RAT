cmake_minimum_required(VERSION 3.19)
project(bot)

set(CMAKE_CXX_STANDARD 20)

# TODO check manual; use "set" or "option" ?
set(BOT_ENABLE ON)      # func "set" is used instead of option(BOT_ENABLE ON)
set(CONTROL_ENABLE OFF) # func "set" is used instead of option(CONTROL_ENABLE OFF)

add_subdirectory(../abstract-core core-lib)
init_bot("../abstract-core" "../modules")

add_executable(bot ${CORE_SOURCES}
        impl/WebSocketRunner.cpp
        impl/TaskManager.cpp
        impl/ModulesManager.cpp
        impl/ApplicationContext.cpp
        impl/Bot.cpp
        main.cpp)
target_link_libraries(bot ${CORE_DEPENDENCIES})