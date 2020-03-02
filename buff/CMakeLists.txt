project(code_test)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../local/lib)

set(BYTE_BUFF byte_buffer.cc)

add_library(buffer ${BYTE_BUFF})

target_link_libraries(buffer thread)