project(code_test)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../local/lib)
aux_source_directory(. DIR_LIB_SRCS)

add_library(thread ${DIR_LIB_SRCS})

target_link_libraries(thread pthread)