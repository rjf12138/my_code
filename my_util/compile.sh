#!/bin/bash

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
#    ret=`ps -A | grep server.sh`
#    echo "$ret"
#    echo "$ret" | gawk "{print $2}" #| gawk -F\/ "{print $1}"
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