cmake_minimum_required(VERSION 3.16)

project("all modules")

macro(get_abstract_module RELATIVE_PATH)
    set(MODULE_SOURCES ${RELATIVE_PATH}module_resources.cpp)
endmacro()


if(BOT_ENABLE)
    add_subdirectory("bot-side")
endif()

if(CONTROL_ENABLE)
    add_subdirectory("control-side")
endif()