project(code_test)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../local/lib)

set(WEJSON wejson.cc json_type.cc)

add_library(wejson ${WEJSON})

target_link_libraries(wejson buffer)
target_link_libraries(wejson file)
target_link_libraries(wejson msg_record)