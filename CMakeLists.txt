#
#Guttemberg Machado on 24/07/17.
#

cmake_minimum_required(VERSION 3.7)
project(dora)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

set(SOURCE_FILES
        main.cpp
        model/model.cpp
        model/model.h
        model/sample.h
        model/sample.cpp
        tools/helper.cpp
        tools/helper.h
        tools/binarization.cpp
        tools/binarization.h
        tools/xycut.cpp
        tools/xycut.h
        model/class.cpp
        model/class.h tools/noise.h)

add_executable(dora ${SOURCE_FILES})

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(dora ${OpenCV_LIBS})

add_custom_command(TARGET dora POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/samples
        $<TARGET_FILE_DIR:dora>/samples)
