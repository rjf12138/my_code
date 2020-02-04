project(code_test)
cmake_minimum_required(VERSION 3.10)

include_directories(./local/include/)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-std=c++11)
    message(STATUS "optional:-std=c++11")   
endif(CMAKE_COMPILER_IS_GNUCXX)

add_subdirectory(./buff)
add_subdirectory(./buff/test)