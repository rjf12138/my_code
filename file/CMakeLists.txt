project(code_test)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../local/lib)

set(FILE file.cc)

add_library(file ${FILE})

target_link_libraries(file buffer)