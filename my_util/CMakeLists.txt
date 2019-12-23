project(util)
cmake_minimum_required(VERSION 3.10)

include_directories(./)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test/bin)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-std=c++11)
    message(STATUS "optional:-std=c++11")   
endif(CMAKE_COMPILER_IS_GNUCXX)

add_definitions("-g")

# 生成链接库
aux_source_directory(. DIR_LIB_SRCS)
add_library (util ${DIR_LIB_SRCS})