cmake_minimum_required(VERSION 3.16)
project(core)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost COMPONENTS thread filesystem)

include_directories(../websocket-lib/)

#add_compile_definitions(ENUM_ARG_ID_FORMS=RESPONSE_TYPE, RPT, PACKAGE_TYPE, PT, REQUEST_ID, REQ, MODULE, M)

set(CORE_SOURCES
        main.cpp
        impl/WebsocketRunner.cpp
        impl/CommandsManager.cpp
        impl/System.cpp
        impl/ApplicationContext.cpp
        impl/ModulesManager.cpp
        models/src/ParsedTextMessage.cpp
        models/src/TaskResult.cpp
        models/src/Task.cpp
        )


add_executable(core ${CORE_SOURCES})


add_subdirectory("modules")

target_link_libraries (core ${Boost_LIBRARIES} dl)