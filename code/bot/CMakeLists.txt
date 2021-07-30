cmake_minimum_required(VERSION 3.19)

project(bot VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Getting abstract core sources and dependencies
add_subdirectory(../abstract-core rat-core)
init_core("../abstract-core" "../modules")


add_executable(bot ${ABSTRACT_CORE_SOURCES}
        impl/WebSocketRunner.cpp
        impl/TaskManager.cpp
        impl/ModulesManager.cpp
        impl/ApplicationContext.cpp
        impl/Bot.cpp
        main.cpp)

target_link_libraries(bot ${ABSTRACT_CORE_DEPENDENCIES})


# [OPTIONAL] Enable building external bot modules
set(BUILD_BOT_MODULES ON)
set(BUILD_CONTROL_MODULES OFF)
add_subdirectory(../modules modules)