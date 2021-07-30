cmake_minimum_required(VERSION 3.16)

project(abstract_core VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(NOT HAS_PARENT)
    message(WARNING "Current scope has no parent.  No targets to build")
    #[[ message(STATUS "Perform test build")
        init_core("." "../modules")
        add_executable(test_control ${CORE_SOURCES} main.cpp)
        target_link_libraries(test_control ${CORE_DEPENDENCIES})]]
endif()

macro(set_core_sources CORE_SOURCES_PATH GLOBAL_SOURCES_PATH)
    set(ABSTRACT_CORE_SOURCES
            ${CORE_SOURCES_PATH}/impl/AbstractWebsocketRunner.cpp
            ${CORE_SOURCES_PATH}/impl/AbstractTaskManager.cpp
            ${CORE_SOURCES_PATH}/impl/AbstractApplicationContext.cpp
            ${CORE_SOURCES_PATH}/impl/AbstractModulesManager.cpp
            ${CORE_SOURCES_PATH}/models/src/ParsedTextMessage.cpp
            ${CORE_SOURCES_PATH}/models/src/TaskInfo.cpp

            ${GLOBAL_SOURCES_PATH}/GlobalResources.cpp)
endmacro()

macro(get_abstract_core_dependencies)
    find_package(Boost COMPONENTS thread filesystem REQUIRED)
    find_package(websocketpp REQUIRED)
    set(ABSTRACT_CORE_DEPENDENCIES ${Boost_LIBRARIES} dl)
endmacro()

macro(init_core CORE_SOURCES_PATH GLOBAL_SOURCES_PATH)
    include_directories("${CORE_SOURCES_PATH}" "${GLOBAL_SOURCES_PATH}")
    set_core_sources("${CORE_SOURCES_PATH}" "${GLOBAL_SOURCES_PATH}")
    get_abstract_core_dependencies()
endmacro()