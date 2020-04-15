#ifndef __WEJSON_H__
#define __WEJSON_H__

#include "basic_head.h"
#include "byte_buffer.h"
#include "file.h"
#include "json_type.h"
// #include "msg_record.h"

namespace my_util {

class WeJson {
public:
    WeJson(void);
    WeJson(string str);
    ~WeJson(void);

    int open_json(string json_file_path);
    int write_json(string json_file_path);
    int parser_from_json(string str);
    int parser_from_json(ByteBuffer &buff);
    string generate_to_json(void);

    ValueTypeCast& operator[](string key);
    ValueTypeCast& operator[](int key);

    string debug_info(void);

public:
    ValueTypeCast json_value_;
    const vector<char> sperate_chars = {' ', '\r', '\n','\t','{', '}','[', ']',',',':','"'};
};

}

#endif