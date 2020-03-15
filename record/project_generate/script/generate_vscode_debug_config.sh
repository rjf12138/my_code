#!/bin/bash

cd $VSCODE_CONFIG

echo "{"                                                                > tasks.json
echo "    # See https://go.microsoft.com/fwlink/?LinkId=733558"         >> tasks.json
echo "    # for the documentation about the tasks.json format"          >> tasks.json
echo "        \"tasks\": ["                                             >> tasks.json
echo "            {"                                                    >> tasks.json
echo "                \"type\": \"shell\","                             >> tasks.json
echo "                \"label\": \"compile script\","                   >> tasks.json
echo "                \"command\": \"$COMPILE_SCRIPT_PATH\","           >> tasks.json
echo "                \"args\": ["                                      >> tasks.json
echo "                    $COMPILE_SCRIPT_ARGS"                         >> tasks.json
echo "                ],"                                               >> tasks.json
echo "                \"options\": {"                                   >> tasks.json
echo "                    \"cwd\": "${workspaceFolder}""                >> tasks.json
echo "                },"                                               >> tasks.json
echo "                \"group\": {"                                     >> tasks.json
echo "                    \"kind\": \"build\","                         >> tasks.json
echo "                    \"isDefault\": true"                          >> tasks.json
echo "                }"                                                >> tasks.json
echo "            }"                                                    >> tasks.json
echo "        ],"                                                       >> tasks.json
echo "        \"version\": \"2.0.0\""                                   >> tasks.json
echo "    }"                                                            >> tasks.json

echo "{"                                                                > launch.json
echo "    # Use IntelliSense to learn about possible attributes."       >> launch.json
echo "    # Hover to view descriptions of existing attributes."         >> launch.json
echo "    # For more information, visit: "                              >> launch.json
echo "    # https://go.microsoft.com/fwlink/?linkid=830387"             >> launch.json
echo "    \"version\": \"0.2.0\","                                      >> launch.json
echo "    \"configurations\": ["                                        >> launch.json 
echo "        {"                                                        >> launch.json
echo "            \"name\": \"gdb_debug\","                             >> launch.json
echo "            \"type\": \"cppdbg\","                                >> launch.json
echo "            \"request\": \"launch\","                             >> launch.json
echo "            \"program\": \"$BIN_OUTPUT_DIR/$EXEC_NAME\","         >> launch.json
echo "            \"args\": $EXEC_ARGS,"                                >> launch.json
echo "            \"stopAtEntry\": false,"                              >> launch.json
echo "            \"cwd\": "${workspaceFolder}","                       >> launch.json
echo "            \"environment\": [],"                                 >> launch.json
echo "            \"externalConsole\": false,"                          >> launch.json
echo "            \"MIMode\": \"gdb\","                                   >> launch.json
echo "            \"setupCommands\": ["                                 >> launch.json
echo "                {"                                                >> launch.json
echo "                    \"description\": \"\","                       >> launch.json
echo "                    \"text\": \"-enable-pretty-printing\","       >> launch.json    
echo "                    \"ignoreFailures\": true"                     >> launch.json
echo "                }"                                                >> launch.json
echo "            ]"                                                    >> launch.json
echo "        }"                                                        >> launch.json
echo "    ]"                                                            >> launch.json
echo "}"                                                                >> launch.json

exit 0