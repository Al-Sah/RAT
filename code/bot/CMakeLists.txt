cmake_minimum_required(VERSION 3.19)
project(bot)

set(CMAKE_CXX_STANDARD 20)

# TODO check manual; use "set" or "option" ?
set(BOT_ENABLE ON)      # func "set" is used instead of option(BOT_ENABLE ON)
set(CONTROL_ENABLE OFF) # func "set" is used instead of option(CONTROL_ENABLE OFF)

add_subdirectory(../core core-lib)
init_bot("../core" "../modules")

add_executable(bot ${CORE_SOURCES} main.cpp)
target_link_libraries(bot ${CORE_DEPENDENCIES})