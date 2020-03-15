#!/bin/bash

cd $PROJECT_PATH

if [ ! -f ./$CMAKE_FILE ]
then
    echo "can't find CMakeLists.txt. path."
fi

if [ -d "./build" ]
then
    rm -rf ./build
fi

# 编译项目
# 项目的可执行文件或是库根据生成的cmake_file
# 决定输出位置
mkdir ./build
cd build
cmake ..
make
cd ..

# 如果设置 -r 选项表示编译完后
# 清理生成的中间文件
if [ $# -eq 1 ] && [ $1 == "-r" ]
then
    ./clean_project.sh
fi

# 将项目下的 config 配置文件拷贝到可执行文件所在目录
cp -rf ./config/* $CONFIG_OUTPUT_DIR

exit 0