#!/bin/bash

cd ./main/
cmake_file=CMakeLists.txt
touch $cmake_file

echo "project($PROJECT_NAME)" >> ./$cmake_file
echo "set(EXECUTABLE_OUTPUT_PATH $BIN_OUTPUT_DIR)" >> ./cmake_file

# 匹配满足 .cc 和 .cpp 后缀的文件
for src_file in `ls src | grep -E ".cc|.cpp"`
do
    add_executable(${EXEC_NAME} $src_file)
done

target_link_libraries($EXEC_NAME $LIB_NAME)
for lib_file in `${$LIB_LINK_LIST[@]}`
do
    target_link_libraries($EXEC_NAME $lib_file)
done

exit 0