#!/bin/bash

if [ -d $1 ]
then
    echo "$1 is exists"
    exit 1
fi

mkdir ./$1/
mkdir ./$1/config
mkdir ./$1/doc
mkdir ./$1/inc
mkdir ./$1/lib
mkdir ./$1/main
mkdir ./$1/src
mkdir ./$1/.proj_config

./generate_proj_cmake_file.sh
./generate_proj_config.sh