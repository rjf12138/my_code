#!/bin/bash

# 在项目的主目录生成主 CMakeLists.txt
# 在 src 目录下生成用于生成库的 CMakeLists.txt
# 在 main 目录下生成用于生成可执行文件的 CMakeLists.txt
cd $PROJECT_PATH
cmake_file=CMakeLists.txt
touch $cmake_file

# 向主目录下的 CMakeLists.txt 写项目信息
echo "project($PROJECT_NAME)" > ./$cmake_file
echo "cmake_minimum_required(VERSION 3.10)" >> ./$cmake_file
echo "include_directories(./inc/)" >> ./$cmake_file
echo "include_directories(\${PROJECT_BINARY_DIR}/../local/include)"
echo "link_directories(\${PROJECT_BINARY_DIR}/../local/lib)" >> ./$cmake_file
echo ""
echo "if(CMAKE_COMPILER_IS_GNUCXX)" >> ./$cmake_file
echo "    add_compile_options(-std=c++11)" >> ./$cmake_file
echo "    message(STATUS \"optional:-std=c++11\")" >>  ./$cmake_file 
echo "endif(CMAKE_COMPILER_IS_GNUCXX)" >> ./$cmake_file
echo ""
echo "add_subdirectory(./src)" >> ./$cmake_file
echo "add_subdirectory(./main)" >> ./$cmake_file

exit 0