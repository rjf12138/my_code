project(code_test)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../local/lib)

set(ERROR_HANDLER msg_record.cc)

add_library(msg_record ${ERROR_HANDLER})