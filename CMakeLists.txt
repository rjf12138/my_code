project(code_test)
cmake_minimum_required(VERSION 3.10)

include_directories(./local/include/)
link_directories(${PROJECT_BINARY_DIR}/../local/lib)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-g -std=c++11)
    message(STATUS "optional:-std=c++11")   
endif(CMAKE_COMPILER_IS_GNUCXX)

add_subdirectory(./file)
add_subdirectory(./msg_record)
add_subdirectory(./thread)
add_subdirectory(./buff)
add_subdirectory(./buff/test)
add_subdirectory(./json)
add_subdirectory(./json/test)