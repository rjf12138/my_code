project(code_test)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../local/lib)

set(ERROR_HANDLER error_handler.cc)

add_library(error ${ERROR_HANDLER})