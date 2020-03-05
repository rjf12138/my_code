project(server_test)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(WeJson_Test WeJson_Test.cc)

target_link_libraries(WeJson_Test wejson)
target_link_libraries(WeJson_Test gtest)
target_link_libraries(WeJson_Test pthread)