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

class ValueTypeCast;

struct JsonObject {
    ValueTypeCast& operator[](string key);
    const ValueTypeCast& operator[](string key) const;

    bool operator==(const JsonObject& rhs) const;
    bool operator!=(const JsonObject& rhs) const;
public:
    map<string, ValueTypeCast> object_val_;
};

struct JsonArray {
    ValueTypeCast& operator[](size_t key);
    const ValueTypeCast& operator[](const size_t key) const;

    bool operator==(const JsonArray& rhs) const;
    bool operator!=(const JsonArray& rhs) const; 
public:
    vector<ValueTypeCast> array_val_;
};

class ValueTypeCast {
public:
    ValueTypeCast(void){}
    ValueTypeCast(double val): type_(NUMBER_TYPE), number_val_(val){}
    ValueTypeCast(int32_t val): type_(NUMBER_TYPE), number_val_(val){}
    ValueTypeCast(string val): type_(STRING_TYPE), str_val_(val){}
    ValueTypeCast(bool val): type_(BOOL_TYPE), bool_val_(val){}
    ValueTypeCast(int* val): type_(NULL_TYPE), null_val_(val){}
    ValueTypeCast(JsonObject val): type_(JSON_OBJECT_TYPE), jobject_val_(val){}
    ValueTypeCast(JsonArray val): type_(JSON_ARRAY_TYPE), jarray_val_(val){}
    ~ValueTypeCast() = default;

    operator double()  {
        if (type_ == NUMBER_TYPE) {
            return number_val_;
        }
    }
    operator bool()  {
        if (type_ == BOOL_TYPE) {
            return bool_val_;
        }
    }
    operator string()   {
        if (type_ == STRING_TYPE) {
            return str_val_;
        }
    }
    operator JsonArray()   {
        if (type_ == JSON_ARRAY_TYPE) {
            return jarray_val_;
        }
    }
    operator JsonObject()  {
        if (type_ == JSON_OBJECT_TYPE) {
            return jobject_val_;
        }
    }

    ValueTypeCast& operator=(double val) {
        type_ = NUMBER_TYPE;
        number_val_ = val;

        return *this;
    }

    ValueTypeCast& operator=(string val) {
        type_ = STRING_TYPE;
        str_val_ = val;
    }

    ValueTypeCast& operator=(bool val) {
        type_ = BOOL_TYPE;
        bool_val_ = val;
        return *this;
    }

    ValueTypeCast& operator=(int* val) {
        type_ = NULL_TYPE;
        null_val_ = val;
        return *this;
    }

    ValueTypeCast& operator=(JsonObject val) {
        type_ = JSON_OBJECT_TYPE;
        jobject_val_ = val;
        return *this;
    }

    ValueTypeCast& operator=(JsonArray val) {
        type_ = JSON_ARRAY_TYPE;
        jarray_val_ = val;
        return *this;
    }

    bool operator==(const double& rhs) {
        if (type_ != NUMBER_TYPE) {
            return false;
        }

        if (rhs != number_val_) {
            return false;
        }

        return true;
    }

    bool operator==(const string& rhs) {
        if (type_ != STRING_TYPE) {
            return false;
        }

        if (rhs != str_val_) {
            return false;
        }

        return true;
    }

    bool operator==(const bool& rhs) {
        if (type_ != BOOL_TYPE) {
            return false;
        }

        if (rhs != bool_val_) {
            return false;
        }

        return true;
    }

    bool operator==(const int*& rhs) {
        if (type_ != NULL_TYPE) {
            return false;
        }

        if (rhs != null_val_) {
            return false;
        }

        return true;
    }

    bool operator==(const JsonObject& rhs) {
        if (type_ != JSON_OBJECT_TYPE) {
            return false;
        }

        if (rhs != jobject_val_) {
            return false;
        }

        return true;
    }

    bool operator==(const JsonArray& rhs) {
        if (type_ != JSON_ARRAY_TYPE) {
            return false;
        }

        if (rhs != jarray_val_) {
            return false;
        }

        return true;
    }

    bool operator==(const ValueTypeCast& rhs) const {
        if (type_ != rhs.type_) {
            return false;
        }

        switch (type_)
        {
            case JSON_OBJECT_TYPE:
            {
                return (jobject_val_ == rhs.jobject_val_);
            } break;
            case JSON_ARRAY_TYPE:
            {
                return (jarray_val_ == rhs.jarray_val_);
            } break;
            case STRING_TYPE:
            {
                return (str_val_ == rhs.str_val_);
            } break;
            case BOOL_TYPE:
            {
                return (bool_val_ == rhs.bool_val_);
            }break;
            case NULL_TYPE:
            {
                return (null_val_ == rhs.null_val_);
            } break;
            case NUMBER_TYPE:
            {
                return (number_val_ == rhs.number_val_);
            } break;
        default:
            break;
        }

        return false;
    }

    bool operator!=(const ValueTypeCast& rhs) const {
        return !(*this == rhs);
    }
public :
    VALUE_TYPE type_;
    double number_val_;
    bool bool_val_;
    string str_val_;
    int *null_val_;
    JsonArray jarray_val_;
    JsonObject jobject_val_;
};

class WeJson {
public:
    WeJson(void);
    WeJson(string str);
    ~WeJson(void);

    int open_json(string json_file_path);
    int parser_from_json(string str);
    int parser_from_json(ByteBuffer &buff);
    int generate_to_json(string &str);

    ValueTypeCast& operator[](const string key);
    const ValueTypeCast& operator[](const string key) const;

private:
    ByteBuffer_Iterator parser_number(ByteBuffer_Iterator start_pos, ValueTypeCast &val);
    ByteBuffer_Iterator parser_null(ByteBuffer_Iterator start_pos, ValueTypeCast &val);
    ByteBuffer_Iterator parser_boolean(ByteBuffer_Iterator start_pos, ValueTypeCast &val);
    ByteBuffer_Iterator parser_string(ByteBuffer_Iterator start_pos, ValueTypeCast &val);
    ByteBuffer_Iterator parser_array(ByteBuffer_Iterator start_pos, ValueTypeCast &val);
    ByteBuffer_Iterator parser_object(ByteBuffer_Iterator start_pos, ValueTypeCast &val);
    ByteBuffer_Iterator parser_common(ByteBuffer_Iterator start_pos);

    VALUE_TYPE check_valuetype(ByteBuffer_Iterator start_pos);

private:
    ByteBuffer raw_json_buffer_;
    ByteBuffer simplify_json_buffer_;
    JsonObject json_object_;
    const vector<char> sperate_chars = {' ', '\r', '\n','\t','{', '}','[', ']',',',':','"'};
};

}

#endif