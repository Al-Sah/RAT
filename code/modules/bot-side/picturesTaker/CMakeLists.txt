cmake_minimum_required(VERSION 3.16)
project(picturesTaker)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(MODULE_CLASS=PicturesTaker MODULE_FOLDER=bot-side/picturesTaker)

add_library(picturesTaker SHARED PicturesTaker.cpp ../../module.cpp)

#add_executable(testPicturesTaker PicturesTaker.cpp ../../module.cpp main.cpp)
