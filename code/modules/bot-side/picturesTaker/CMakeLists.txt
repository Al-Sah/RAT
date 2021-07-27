cmake_minimum_required(VERSION 3.16)
project(picturesTaker)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(MODULE_CLASS=PicturesTaker MODULE_FOLDER=bot-side/picturesTaker)

add_library(picturesTaker SHARED PicturesTaker.cpp ../../Module.cpp)

#add_executable(testPicturesTaker PicturesTaker.cpp ../../module.cpp main.cpp)

function(get_pictures_taker PATH)
    install(TARGETS picturesTaker LIBRARY DESTINATION ${PATH})
endfunction()