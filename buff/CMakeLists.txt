project(code_test)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(BYTE_BUFF byte_buffer.cc)

add_library(buffer SHARED ${BYTE_BUFF})