#ifndef __WEJSON_H__
#define __WEJSON_H__

#include "basic_head.h"
#include "byte_buffer.h"
#include "file.h"
#include "json_type.h"

namespace my_util {

class WeJson : public MsgRecord {
public:
    WeJson(void);
    WeJson(string str);
    ~WeJson(void);

    // 从文件中读取json文本
    int open_json(string json_file_path);
    // 将json写入到文件中
    int write_json(string json_file_path);
    // 解析字符串中保存的json文本
    int parser_from_json(string str);
    // 解析ByteBuffer保存的json文本
    int parser_from_json(ByteBuffer &buff);

    // 将json转换为文本输出
    string generate_to_json(void);
    // 该类中保存的是json的object类型，采用key-value的方式访问
    ValueTypeCast& operator[](string key);
    // 该类中保存的是json的array类型，采用索引的方式访问
    ValueTypeCast& operator[](int index);

    // 当出错时输出调试信息
    string debug_info(void);

public:
    ValueTypeCast json_value_;
    // json中重要的分割字符
    const vector<char> sperate_chars = {' ', '\r', '\n','\t','{', '}','[', ']',',',':','"'};
};

}

#endif