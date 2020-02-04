project(server_test)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(ByteBuffer_Test ByteBuffer_Test.cc)

target_link_libraries(ByteBuffer_Test buff)