cmake_minimum_required(VERSION 3.16)

project("all modules")

macro(get_abstract_module RELATIVE_PATH)
    set(MODULE_SOURCES ${RELATIVE_PATH}GlobalResources.cpp)
endmacro()


if(BUILD_BOT_MODULES)
    set(ROLE bot-side)
elseif(BUILD_CONTROL_MODULES)
    set(ROLE control-side)
    add_subdirectory(server-interaction/${ROLE})
endif()


if(BUILD_BOT_MODULES OR BUILD_CONTROL_MODULES)
    add_subdirectory(echo/${ROLE})
    add_subdirectory(picturesTaker/${ROLE})
endif()