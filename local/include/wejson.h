#ifndef __WEJSON_H__
#define __WEJSON_H__

#include "basic_head.h"
#include "byte_buffer.h"
#include "file.h"

namespace my_util {

enum VALUE_TYPE {
    UNKNOWN_TYPE = -1000,
    NUMBER_TYPE = 10001,
    BOOL_TYPE,
    NULL_TYPE,
    STRING_TYPE,
    JSON_ARRAY_TYPE,
    JSON_OBJECT_TYPE
};

struct JsonArray;
struct JsonObject;

struct value_type {
    int type_;
    int number_val_;
    bool bool_val_;
    string null_val_;
    string str_val_;
    JsonArray jarray_val_;
    JsonObject jobject_val_;
};

class ValueTypeCast {
public:
    ValueTypeCast(value_type vt): value_type_(vt) {}
    ~ValueTypeCast() = default;

    void set_value(value_type vt) {
        value_type_ = vt;
    }

    operator int() const {
        if (value_type_.type_ == NUMBER_TYPE) {
            return value_type_.number_val_;
        }
    }
    operator bool() const {
        if (value_type_.type_ == BOOL_TYPE) {
            return value_type_.bool_val_;
        }
    }
    operator string() const  {
        if (value_type_.type_ == STRING_TYPE) {
            return value_type_.str_val_;
        }
    }
    operator JsonArray() const  {
        if (value_type_.type_ == JSON_ARRAY_TYPE) {
            return value_type_.jarray_val_;
        }
    }
    operator JsonObject() const {
        if (value_type_.type_ == JSON_OBJECT_TYPE) {
            return value_type_.jobject_val_;
        }
    }

    operator JsonObject() const {
        if (value_type_.type_ == JSON_OBJECT_TYPE) {
            return value_type_.jobject_val_;
        }
    }

public :
    value_type value_type_;
};

struct JsonObject {
    ValueTypeCast& operator[](string key) {
        if (object_val_.find(key) == object_val_.end()) {
            ostringstream str_output;
            str_output << "key: " << key << " not exists." << std::endl;
            throw runtime_error(str_output.str());
        } else {
            return object_val_[key];
        }
    }
    const ValueTypeCast& operator[](const string key) const {
        auto iter = object_val_.find(key);
        if (iter == object_val_.end()) {
            
            stringstream str_output;
            str_output << "key: " << key << " not exists." << std::endl;
            throw runtime_error(str_output.str());
            return iter->second;
        } else {
            return iter->second;
        }
        // https://www.cnblogs.com/kuliuheng/p/5738703.html
    }
public:
    map<string, ValueTypeCast> object_val_;
};

struct JsonArray {
    ValueTypeCast& operator[](size_t key) {
        if (key > array_val_.size()) {
            ostringstream str_output;
            str_output << "pos: " << key << " out of range." << std::endl;
            throw runtime_error(str_output.str());
        } else {
            return array_val_[key];
        }
    }
    const ValueTypeCast& operator[](const size_t key) const {
        if (key > array_val_.size()) {
            ostringstream str_output;
            str_output << "pos: " << key << " out of range." << std::endl;
            throw runtime_error(str_output.str());
        } else {
            return array_val_[key];
        }
    }

    void push_back(value_type val) {
        array_val_.push_back(val);
    }
public:
    vector<ValueTypeCast> array_val_;
};

class WeJson {
public:
    WeJson(void);
    WeJson(string str);
    WeJson(string json_file_path);
    ~WeJson(void);

    bool check_json(ByteBuffer &buff);
    int open_json(string json_file_path);
    int parser_from_json(string str);
    int parser_from_json(ByteBuffer &buff);
    int generate_to_json(string &str);

    ValueTypeCast& operator[](const string key);
    const ValueTypeCast& operator[](const string key) const;

private:
    ByteBuffer_Iterator parser_number(ByteBuffer_Iterator start_pos, value_type &val);
    ByteBuffer_Iterator parser_null(ByteBuffer_Iterator start_pos, value_type &val);
    ByteBuffer_Iterator parser_boolean(ByteBuffer_Iterator start_pos, value_type &val);
    ByteBuffer_Iterator parser_string(ByteBuffer_Iterator start_pos, value_type &val);
    ByteBuffer_Iterator parser_array(ByteBuffer_Iterator start_pos, value_type &val);
    ByteBuffer_Iterator parser_object(ByteBuffer_Iterator start_pos, value_type &val);
    ByteBuffer_Iterator parser_common(ByteBuffer_Iterator start_pos);

    string escape_string(string raw_str); // 字符串转义在此处处理
    VALUE_TYPE check_valuetype(ByteBuffer_Iterator start_pos);

private:
    ByteBuffer json_buffer_;
    JsonObject json_object_;
    const vector<char> sperate_chars = {' ', '\r', '\n','\t','{', '}','[', ']',',',':','"'};
};

}

#endif