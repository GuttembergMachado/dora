cmake_minimum_required(VERSION 3.7)
project(dora)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp ./tools/helper.cpp ./tools/helper.h ./model/model.cpp ./model/model.h)
add_executable(dora ${SOURCE_FILES})