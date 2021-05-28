cmake_minimum_required(VERSION 3.19)
project(server_interaction)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(MODULE_CLASS=serverInteraction MODULE_FOLDER=control-side/server-interaction)

add_library(server_interaction SHARED serverInteraction.cpp ../../module.cpp)

#add_executable(server_interaction_  serverInteraction.cpp ../../module.cpp main.cpp)
target_link_libraries (server_interaction Qt5::Widgets)
