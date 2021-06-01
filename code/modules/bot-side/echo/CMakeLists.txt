cmake_minimum_required(VERSION 3.16)
project(echo)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(MODULE_CLASS=EchoModule MODULE_FOLDER=bot-side/echo)


add_library(echo SHARED EchoModule.cpp ../../module.cpp)
