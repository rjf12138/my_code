#!/bin/bash

function help_info() 
{
    echo "Usage: $0 [OPTIONS] [param1 param2 ...]"
    echo "-h                    --print help info."
    echo "-cr                   --clean and rebuild project."
    echo "-r                    --rebuild without cleanning."
    echo "-c                    --clean all."
    echo "-e                    --enter project exec file dir."
    echo "-w                    --open new gnome_terminal."
    echo "-cp proj_name         --create new project."
    echo "-sp project_name      --set current work project."
    echo ""

    exit 0
}

PROJECT_CONFIG=project.ini
SCRIPT_PATH=`cat $PROJECT_CONFIG | grep SCRIPT_PATH | awk -F= '{print $2}'`
PROJECT_NAME=`cat $PROJECT_CONFIG | grep CURR_PROJ | awk -F= '{print $2}'`

function enter_bin_directory()
{
    dir=$BIN_OUTPUT_DIR
    if [ ! -d "$dir" ]
    then
        echo "Can't find \"$dir\" directory."
        exit 0
    fi
    cd $dir
    exec gnome-terminal
}

function set_curr_project()
{
    sed "s/$PROJECT_NAME/$2/g" -i $PROJECT_CONFIG
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
    source ./$PROJECT_NAME/.proj_config/proj_config.sh
    $SCRIPT_PATH/compile_and_install.sh -cr
    ;;
"-r")
    source ./$PROJECT_NAME/.proj_config/proj_config.sh
    $SCRIPT_PATH/compile_and_install.sh
    ;;
"-c")
    source ./$PROJECT_NAME/.proj_config/proj_config.sh
    ./clean_project.sh
    ;;
"-e")
    if [ $# -ne 2 ]
    then
        help_info
    fi
    source ./$PROJECT_NAME/.proj_config/proj_config.sh
    enter_bin_directory $2
    ;;
"-w")
    gnome-terminal
    ;;
"-cp")
    if [ $# -ne 2 ]
    then
        help_info
    fi
    ./generate_project.sh $2
    set_curr_project $2
    ;;
"-cp")
    if [ $# -ne 2 ]
    then
        help_info
    fi
    set_curr_project $2
    ;;
*)
    help_info
    ;;
esac

exit 0