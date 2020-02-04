#!/bin/bash

# if [ $# -ne 1 ]
# then
#     echo "$0 dir_name"
#     exit 1
# fi
# 
# if [ ! -d $1 ]
# then
#     echo "$1 is not exists."
#     exit 1
# fi

# if [ ! -f "CMakeLists.txt" ]
# then
#     echo "Can't find CMakeLists.txt."
#     exit 1
# fi
# 
# # 因为替换的的文本内有 ‘/’ 字符，所以sed的分隔符需要使用改为 #
# sed -i "s#add_subdirectory(src)#add_subdirectory(./$1)#g" CMakeLists.txt
# sed -i "s#add_subdirectory(test)#add_subdirectory(./$1/test)#g" CMakeLists.txt

function help_info() 
{
    echo "Usage: $0 [ OPTIONS ]"
    echo "OPTIONS := { -h[help] | -cr[clean and rebuild] | -r[rebuild] |"
    echo            "            -c[clean all] -e[enter bin directory] | -w[new window]}"
    exit 0
}

function clean_all()
{
    if [ -d "build" ]
    then
        rm -r ./build
    fi 
}

function compile()
{
    if [ ! -d "build" ]
    then
        mkdir build
    fi
#    cp -r ./server_config/ ./build
    cd build

    cmake ..
    make
}

function enter_bin_directory()
{
    if [ ! -d "build" ]
    then
        echo "Can't find \"build\" directory."
        exit 0
    fi
    cd build/test/bin
    exec gnome-terminal
}

if [ $# -eq 0 ]
then
    help_info
fi

case $1 in
"-h")
    help_info
    ;;
"-cr")
    clean_all
    compile
    ;;
"-r")
    compile
    ;;
"-c")
    clean_all
    ;;
"-e")
    enter_bin_directory
    ;;
"-w")
    gnome-terminal
    ;;
*)
    help_info
    ;;
esac

exit 0

exit 0