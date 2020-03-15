#!/bin/bash

cd $PROJECT_PATH/src/
touch $CMAKE_FILE

echo "project($PROJECT_NAME)" >> ./$CMAKE_FILE
echo "set(LIBRARY_OUTPUT_PATH $LIB_OUTPUT_DIR)" >> ./CMAKE_FILE

# 匹配满足 .cc 和 .cpp 后缀的文件
for src_file in `ls src | grep -E ".cc|.cpp"`
do
    add_library($LIB_NAME $src_file)
done

for lib_file in `${$LIB_LINK_LIST[@]}`
do
    target_link_libraries($LIB_NAME $lib_file)
done

exit 0