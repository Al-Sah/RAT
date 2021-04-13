cmake_minimum_required(VERSION 3.16)
project(echo)

set(CMAKE_CXX_STANDARD 20)
add_compile_definitions(MODULE_CLASS=EchoModule)

add_library(echo SHARED EchoModule.cpp ../main.cpp)
