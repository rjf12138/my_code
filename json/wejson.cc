#include "wejson.h"

namespace my_util {
WeJson::WeJson(void) = default;
WeJson::WeJson(string str) 
{
    this->parser_from_json(str);
}
WeJson::WeJson(string json_file_path)
{
    this->open_json(json_file_path);
}
WeJson::~WeJson(void)
{

}

void 
WeJson::open_json(string json_file_path)
{
    FileOperate json_file;
    if (json_file.open(json_file_path) == -1) {
        ostringstream ostr;
        ostr << "Line: " << __LINE__ << "file: " << json_file_path << " is not exists.";
        throw runtime_error(ostr.str());
    }

    struct stat file_info;
    off_t file_size = json_file.fileinfo(file_info);
    json_file.read(json_buffer_, file_info.st_size);
    this->parser_from_json(json_buffer_);
}

int 
WeJson::segmentate_string(map<char, vector<ByteBuffer_Iterator>> &ret_pos)
{
    ByteBuffer buffer;
    for (auto iter = json_buffer_.begin(); iter != json_buffer_.end(); ++iter) {
        for (int i = 0; i < 4; ++i) {
            if (*iter == sperate_chars[i]) {
                continue;
            }
        }
        buffer.write_int8(*iter);
    }

    value_type ret_json;
    this->parser_object(buffer.begin(), ret_json);
    json_object_ = ret_json.jobject_val_;

    return 0;
}


ByteBuffer_Iterator 
WeJson::parser_number(ByteBuffer_Iterator start_pos, value_type &val)
{
    ByteBuffer_Iterator iter = start_pos;
    string str;
    for (; iter != json_buffer_.end(); ++iter) {
        if (*iter > '9' || *iter < '0') {
            break;
        }
        str += *iter;
    }
    val.type_ = INT_TYPE;
    val.int_val_ = stoi(str, 0, str.length());
    return iter;
}
ByteBuffer_Iterator 
WeJson::parser_boolean(ByteBuffer_Iterator start_pos, value_type &val)
{
    ByteBuffer_Iterator iter = start_pos;
    int read_size = 0;
    if (*iter == 't') {
        read_size = 4;
    } else if (*iter == 'f') {
        read_size = 5;
    }

    string str;
    for (int i = 0; i < read_size; ++i) {
        str += *iter;
        ++iter;
    }

    val.type_ = BOOL_TYPE;
    if (str == "true") {
        val.bool_val_ = true;
    } else if (str == "false") {
        val.bool_val_ = false;
    }

    return iter;
}

ByteBuffer_Iterator 
WeJson::parser_null(ByteBuffer_Iterator start_pos, value_type &val)
{
    ByteBuffer_Iterator iter = start_pos;
    int read_size = 0;
    if (*iter == 'n') {
        read_size = 4;
    }

    string str;
    for (int i = 0; i < read_size; ++i) {
        str += *iter;
        ++iter;
    }

    val.type_ = NULL_TYPE;
    if (str == "null") {
        val.null_val_ = "null";
    }

    return iter;
}

ByteBuffer_Iterator 
WeJson::parser_string(ByteBuffer_Iterator start_pos, value_type &val)
{
    ByteBuffer_Iterator iter = start_pos;
    ++iter; // 此时 *iter 值为 "

    string str;
    for (; *iter != '"' || iter != json_buffer_.end(); ++iter) {
        str += *iter;
    }

    if (iter == json_buffer_.end())
    {
        // TODO
    }

    val.type_ = STRING_TYPE;
    val.str_val_ = this->escape_string(str);
    ++iter; // 此时 iter 值为 "

    return iter;
}

ByteBuffer_Iterator 
WeJson::parser_array(ByteBuffer_Iterator start_pos, value_type &val)
{
    val.type_ = JSON_ARRAY_TYPE;
    ByteBuffer_Iterator iter = start_pos;
    ++iter;

    value_type ret_value;
    for (; *iter != ']' || iter != json_buffer_.end(); ++iter) {
        VALUE_TYPE ret_value_type = this->check_valuetype(iter);
        if (ret_value_type == UNKNOWN_TYPE) {
            continue;
        }
        switch (ret_value_type)
        {
            case JSON_OBJECT_TYPE:
            {
                iter = this->parser_object(iter, ret_value);
            } break;
            case JSON_ARRAY_TYPE:
            {
                iter = this->parser_array(iter, ret_value);
            } break;
            case STRING_TYPE:
            {
                iter = this->parser_string(iter, ret_value);
            } break;
            case BOOL_TYPE:
            {
                iter = this->parser_boolean(iter, ret_value);
            }break;
            case NULL_TYPE:
            {
                iter = this->parser_null(iter, ret_value);
            } break;
            case INT_TYPE:
            {
                iter = this->parser_number(iter, ret_value);
            } break;
        default:
            break;
        }

        val.jarray_val_.push_back(ret_value);
    }
    if (iter == json_buffer_.end()) {
        // TODE
    }
    ++iter;
    return iter;
}

ByteBuffer_Iterator 
WeJson::parser_object(ByteBuffer_Iterator start_pos, value_type &val)
{
    val.type_ = JSON_OBJECT_TYPE;
    ByteBuffer_Iterator iter = start_pos;
    ++iter;

    value_type ret_value;
    string value_name;
    bool flag = false;
    for (; *iter != '}' || iter != json_buffer_.end(); ++iter) {
        VALUE_TYPE ret_value_type = this->check_valuetype(iter);
        if (ret_value_type == UNKNOWN_TYPE) {
            continue;
        }
        if (flag == false && ret_value_type == STRING_TYPE) {
            iter = this->parser_string(iter, ret_value);
            value_name = ret_value.str_val_;
            flag = true;
        }
        switch (ret_value_type)
        {
            case JSON_OBJECT_TYPE:
            {
                iter = this->parser_object(iter, ret_value);
            } break;
            case JSON_ARRAY_TYPE:
            {
                iter = this->parser_array(iter, ret_value);
            } break;
            case STRING_TYPE:
            {
                iter = this->parser_string(iter, ret_value);
            } break;
            case BOOL_TYPE:
            {
                iter = this->parser_boolean(iter, ret_value);
            }break;
            case NULL_TYPE:
            {
                iter = this->parser_null(iter, ret_value);
            } break;
            case INT_TYPE:
            {
                iter = this->parser_number(iter, ret_value);
            } break;
        default:
            break;
        }
        if (val.jobject_val_.object_val_.find(value_name) == val.jobject_val_.object_val_.end()) {
            val.jobject_val_[value_name] = ret_value;
        } else {
            // TODO
        }
        flag = true;
    }

    ++iter;
    return iter;
}

VALUE_TYPE 
WeJson::check_valuetype(ByteBuffer_Iterator start_pos)
{
    if (*start_pos >= '0' && *start_pos <= '9') {
        return INT_TYPE;
    }
    switch (*start_pos)
    {
        case '{':
        {
            return JSON_OBJECT_TYPE;
        } break;
        case '[':
        {
            return JSON_ARRAY_TYPE;
        } break;
        case '"':
        {
            return STRING_TYPE;
        } break;
        case 't':
        {
            return BOOL_TYPE;
        }break;
        case 'f':
        {
            return BOOL_TYPE;
        }break;
        case 'n':
        {
            return NULL_TYPE;
        } break;
        default:
            break;
    }

    return UNKNOWN_TYPE;
}

void parser_from_json(ByteBuffer &buff)
{

}

}