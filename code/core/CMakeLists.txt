cmake_minimum_required(VERSION 3.16)
project(core)

set(CMAKE_CXX_STANDARD 20)

option(PERFORM_TEST_BUILD "PERFORM_TEST_BUILD" OFF)
option(HEADERS_INCLUDES_ENABLE "HEADERS_INCLUDES_ENABLE" OFF)# TODO choose one realisation...

message(STATUS "/------------options-------------\\")
message(STATUS "| USE_HEADERS_INCLUDES option: ${HEADERS_INCLUDES_ENABLE}")
message(STATUS "| PERFORM_TEST_BUILD   option: ${PERFORM_TEST_BUILD}")
message(STATUS "| CONTROL_ENABLE       option: ${CONTROL_ENABLE}")
message(STATUS "| BOT_ENABLE           option: ${BOT_ENABLE}")
message(STATUS "\\--------------------------------/\n")

add_subdirectory("../modules" "modules")

function(set_default_core_sources PATH)
    set(CORE_SOURCES
            ${PATH}impl/WebsocketRunner.cpp
            ${PATH}impl/CommandsManager.cpp
            ${PATH}impl/System.cpp
            ${PATH}impl/ApplicationContext.cpp
            ${PATH}impl/ModulesManager.cpp
            ${PATH}models/src/ParsedTextMessage.cpp
            ${PATH}models/src/TaskResult.cpp
            ${PATH}models/src/TaskInfo.cpp PARENT_SCOPE)
endfunction()

macro(set_bot_sources PATH)
    set_default_core_sources("${PATH}")
    if("${CORE_SOURCES}" EQUAL "")
        message(FATAL_ERROR "*** FAILED TO LOAD CORE_SOURCES")
    endif()
endmacro()

macro(set_control_sources PATH)
    set_default_core_sources("${PATH}")
    set(CORE_SOURCES ${CORE_SOURCES} ${PATH}impl/DataTransfer.cpp ${PATH}DataTransfer.h)
    if("${CORE_SOURCES}" EQUAL "")
        message(FATAL_ERROR "*** FAILED TO LOAD CORE_SOURCES")
    endif()
endmacro()

macro(get_control_dependencies)
    find_package(Boost COMPONENTS thread filesystem REQUIRED)
    find_package(websocketpp REQUIRED)
    find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
    set(CORE_DEPENDENCIES ${Boost_LIBRARIES} dl Qt5::Widgets)

endmacro()

macro(get_bot_dependencies)
    find_package(Boost COMPONENTS thread filesystem REQUIRED)
    find_package(websocketpp REQUIRED)
    set(CORE_DEPENDENCIES ${Boost_LIBRARIES} dl)
endmacro()

macro(append_module_sources RELATIVE_PATH_MODULE_SOURCES)
    get_abstract_module("${RELATIVE_PATH_MODULE_SOURCES}")
    set(CORE_SOURCES ${CORE_SOURCES} ${MODULE_SOURCES})
endmacro()

macro(init_bot RELATIVE_PATH_BOT_SOURCES RELATIVE_PATH_MODULE_SOURCES)
    get_bot_dependencies()
    include_directories(${RELATIVE_PATH_BOT_SOURCES} ${RELATIVE_PATH_MODULE_SOURCES})
    set_bot_sources("${RELATIVE_PATH_BOT_SOURCES}/")
    append_module_sources("${RELATIVE_PATH_MODULE_SOURCES}/")

    message(STATUS "*** CORE_SOURCES ${CORE_SOURCES}")
    message(STATUS "*** CORE_DEPENDENCIES: ${CORE_DEPENDENCIES}")
endmacro()

macro(init_control RELATIVE_PATH_CONTROL_SOURCES RELATIVE_PATH_MODULE_SOURCES)
    get_control_dependencies()
    include_directories(${RELATIVE_PATH_CONTROL_SOURCES} ${RELATIVE_PATH_MODULE_SOURCES})
    set_control_sources("${RELATIVE_PATH_CONTROL_SOURCES}/")
    append_module_sources("${RELATIVE_PATH_MODULE_SOURCES}/")

    message(STATUS "*** CORE_SOURCES ${CORE_SOURCES}")
    message(STATUS "*** CORE_DEPENDENCIES: ${CORE_DEPENDENCIES}")
endmacro()


if(BOT_ENABLE AND CONTROL_ENABLE)
    message(FATAL_ERROR "*** Both BOT_ENABLE and CONTROL_ENABLE options are ON.\n\tOne option have to be ON and another one OFF")
elseif(NOT BOT_ENABLE AND NOT CONTROL_ENABLE)
    message(FATAL_ERROR "*** Both BOT_ENABLE and CONTROL_ENABLE options are OFF.\n\tOne option have to be ON and another one OFF")
else()
    if(CONTROL_ENABLE)
        message(STATUS "*** ACTIVE TARGET - \"CONTROL\" application")
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTORCC ON)

        if(PERFORM_TEST_BUILD)
            message(STATUS "/-------PERFORM_CONTROL_TEST_BUILD-----\\")
            get_control_dependencies()
            set_control_sources("")
            append_module_sources("../modules/")
            include_directories("../modules")
            add_executable(test_control ${CORE_SOURCES} main.cpp)
            target_link_libraries(test_control ${CORE_DEPENDENCIES})
            message(STATUS "\\------TEST_CONTROL_BUILD_FINISHED-----/\n")
        endif()
    else()
        message(STATUS "*** ACTIVE TARGET - \"BOT\" application !!!\n")
        if(PERFORM_TEST_BUILD)
            message(STATUS "/-------PERFORM_BOT_TEST_BUILD-----\\")
            get_bot_dependencies()
            set_bot_sources("")
            append_module_sources("../modules/")
            include_directories("../modules")
            add_executable(test_bot ${CORE_SOURCES}  main.cpp)
            target_link_libraries(test_bot ${CORE_DEPENDENCIES})
            message(STATUS "\\------TEST_BOT_BUILD_FINISHED-----/\n")
        endif()
    endif()
    configure_file(cmake_configuration.h.in ${CMAKE_CURRENT_SOURCE_DIR}/cmake_configuration.h)
endif()

get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(NOT HAS_PARENT)
    message(WARNING "Current scope has no parent")
endif()



#[[get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()]]
