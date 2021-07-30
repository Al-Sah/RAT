cmake_minimum_required(VERSION 3.16)

project("all modules")

macro(get_abstract_module RELATIVE_PATH)
    set(MODULE_SOURCES ${RELATIVE_PATH}GlobalResources.cpp)
endmacro()


if(BUILD_BOT_MODULES)
    add_subdirectory("bot-side")
endif()

if(BUILD_CONTROL_MODULES)
    add_subdirectory("control-side")
endif()