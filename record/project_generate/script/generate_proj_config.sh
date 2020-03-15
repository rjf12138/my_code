#!/bin/bash

if [ $# -ne 1 ]
then
    echo "$0 project_name"
    exit 1
fi

project_path=`pwd`
project_name=$1

cd ./.proj_config
touch proj_config.sh
echo "#!/bin/bash" > proj_config.sh
echo "" >> proj_config.sh
echo "# basic config"
echo "export VSCODE_CONFIG_PATH=$project_path/../.vscode"
echo "export PROJECT_PATH=$project_path" >> proj_config.sh
echo "export PROJECT_NAME=$project_name" >> proj_config.sh
echo "export EXEC_NAME=${project_name}" >> proj_config.sh
echo "export EXEC_ARGS=\"\"" >> proj_config.sh 
echo "export LIB_NAME=${project_name}" >> proj_config.sh
echo "export CMAKE_FILE=CMakeLists.txt" >> proj_config.sh
echo "export LIB_LINK_LIST=()" >> proj_config.sh
echo "export LIB_OUTPUT_DIR=$project_path/../local/lib" >> proj_config.sh
echo "export BIN_OUTPUT_DIR=$project_path/../local/tmp_bin" >> proj_config.sh
echo "export CONFIG_OUTPUT_DIR=$BIN_OUTPUT_DIR" >> proj_config.sh
echo "" >> proj_config.sh
echo "#compile and debug config"
echo "export COMPILE_SCRIPT_NAME=project.sh"
echo "export COMPILE_SCRIPT_PATH=$project_path/../$COMPILE_SCRIPT_NAME" >> proj_config.sh
echo "export COMPILE_SCRIPT_ARGS=\"\"" >> proj_config.sh
echo "" >> proj_config.sh
echo "exit 0" >> proj_config.sh

chmod u+x $proj_config.sh

exit 0