#!/bin/bash

cd $PROJECT_PATH

if [ -d "./build" ]
then
    rm -rf ./build
fi