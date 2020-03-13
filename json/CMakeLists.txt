project(code_test)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../local/lib)

set(WEJSON wejson.cc)

add_library(wejson ${WEJSON})

target_link_libraries(wejson buffer)
target_link_libraries(wejson file)