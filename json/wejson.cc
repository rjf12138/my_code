#include "wejson.h"

namespace my_util {
WeJson::WeJson(void) = default;
WeJson::WeJson(string str) 
{
    this->parser_from_json(str);
}

WeJson::~WeJson(void)
{

}

int
WeJson::open_json(string json_file_path)
{
    FileOperate json_file;
    if (json_file.open(json_file_path) == -1) {
        ostringstream ostr;
        ostr << "Line: " << __LINE__ << " file: " << json_file_path << " is not exists.";
        throw runtime_error(ostr.str());
    }

    struct stat file_info;
    off_t file_size = json_file.fileinfo(file_info);
    json_file.read(raw_json_buffer_, file_info.st_size);
    this->parser_from_json(raw_json_buffer_);
}

int 
WeJson::parser_from_json(ByteBuffer &buff)
{
    bool quotation_marks = false;
    cout << "data_size: " << buff.data_size() << endl;
    for (auto iter = buff.begin(); iter != buff.end(); ++iter) {
        cout << *iter;
    }
    cout << endl;
    int i = 0;
    for (auto iter = buff.begin(); iter != buff.end(); ++iter) {
        if (quotation_marks == false && *iter == '"') {
            quotation_marks = true;
        } else if (quotation_marks == true && *iter == '"' && *(iter-1) != '\\') {
            quotation_marks = false;
        }
        for (i = 0; i < 4; ++i) {
            if (sperate_chars[i] == *iter) {
                break;
            }
        }
        if (i < 4 && quotation_marks == false) {
            continue;
        }
        simplify_json_buffer_.write_int8(*iter);
    }
    for (auto iter = simplify_json_buffer_.begin(); iter != simplify_json_buffer_.end(); ++iter) {
        cout << *iter;
    }
    cout << endl;
    cout << "parse start" << endl;
    ValueTypeCast ret_json;
    this->parser_object(simplify_json_buffer_.begin(), ret_json);
    json_object_ = ret_json.jobject_val_;
    cout << "parse end" << endl;
    return 0;
}

int 
WeJson::parser_from_json(string str)
{
    raw_json_buffer_.write_bytes(str.c_str(), str.length());
    this->parser_from_json(raw_json_buffer_);
}

ByteBuffer_Iterator 
WeJson::parser_number(ByteBuffer_Iterator start_pos, ValueTypeCast &val)
{
    ByteBuffer_Iterator iter = start_pos;
    string str;
    bool is_decimal = false;
    for (; iter != simplify_json_buffer_.end() || !isdigit(*iter); ++iter) {
        if (*iter == '0' && iter == start_pos) {
            if (*(iter+1) != '.') {
                ostringstream ostr;
                ostr << "Line: " << __LINE__ << " Zero can't be first number of integer!";
                throw runtime_error(ostr.str());
            } else {
                is_decimal = true;
            } 
        }
        str += *iter;
    }

    val.type_ = NUMBER_TYPE;
    if (is_decimal == true) {
        val.number_val_ = stod(str);
    } else {
        val.number_val_ = stoi(str);
    }

    return iter;
}
ByteBuffer_Iterator 
WeJson::parser_boolean(ByteBuffer_Iterator start_pos, ValueTypeCast &val)
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
    } else {
        ostringstream ostr;
        ostr << "Line: " << __LINE__ << " Expected true or false, but result is " << str;
        throw runtime_error(ostr.str());
    }

    return iter;
}

ByteBuffer_Iterator 
WeJson::parser_null(ByteBuffer_Iterator start_pos, ValueTypeCast &val)
{
    ByteBuffer_Iterator iter = start_pos;
    int read_size = 4; // null is four bytes.

    string str;
    for (int i = 0; i < read_size; ++i) {
        str += *iter;
        ++iter;
    }

    val.type_ = NULL_TYPE;
    if (str == "null") {
        val.null_val_ = nullptr;
    } else {
        ostringstream ostr;
        ostr << "Line: " << __LINE__ << " Expected null, but result is " << str;
        throw runtime_error(ostr.str());
    }


    return iter;
}

ByteBuffer_Iterator 
WeJson::parser_string(ByteBuffer_Iterator start_pos, ValueTypeCast &val)
{
    ByteBuffer_Iterator iter = start_pos;
    ++iter; // 此时 *iter 值为 ",指向字符串的第一个字符
    cout << *iter << endl;
    string str;
    for (; iter != simplify_json_buffer_.end(); ++iter) {
        if (*iter == '"' && *(iter - 1) != '\\') {
            break;
        }
        str += *iter;
    }

    if (iter == simplify_json_buffer_.end())
    {
        ostringstream ostr;
        ostr << "Line: " << __LINE__ << " String need to surround by \"\" " << str;
        throw runtime_error(ostr.str());
    }
    cout << str << std::endl;
    val.type_ = STRING_TYPE;
    val.str_val_ = str;
    ++iter; //  iter 指向下一个字符

    return iter;
}

ByteBuffer_Iterator 
WeJson::parser_array(ByteBuffer_Iterator start_pos, ValueTypeCast &val)
{
    val.type_ = JSON_ARRAY_TYPE;
    ByteBuffer_Iterator iter = start_pos;
    ++iter;

    ValueTypeCast ret_value;
    for (; *iter != ']' || iter != simplify_json_buffer_.end(); ++iter) {
        VALUE_TYPE ret_value_type = this->check_valuetype(iter);
        if (ret_value_type == UNKNOWN_TYPE) {
            if (*iter != ',') {
                ostringstream ostr;
                ostr << "Line: " << __LINE__ << " Unknown character in array: " << *iter;
                throw runtime_error(ostr.str());
            }
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
            case NUMBER_TYPE:
            {
                iter = this->parser_number(iter, ret_value);
            } break;
        default:
            break;
        }

        val.jarray_val_.array_val_.push_back(ret_value);
    }
    if (iter == simplify_json_buffer_.end()) {
        ostringstream ostr;
        ostr << "Line: " << __LINE__ << " Array need to surround by []";
        throw runtime_error(ostr.str());
    }
    ++iter;
    return iter;
}

ByteBuffer_Iterator 
WeJson::parser_object(ByteBuffer_Iterator start_pos, ValueTypeCast &val)
{
    val.type_ = JSON_OBJECT_TYPE;
    ByteBuffer_Iterator iter = start_pos;
    ++iter; // 指向第一个键值对
    cout << __LINE__ << endl;
    ValueTypeCast ret_value;
    string value_name;
    bool flag = false;
    for (; *iter != '}' && iter != simplify_json_buffer_.end(); ++iter) {
        cout << *iter << endl;
        VALUE_TYPE ret_value_type = this->check_valuetype(iter);
        if (ret_value_type == UNKNOWN_TYPE) {
            if (*iter != ',' || *iter != ':') {
                ostringstream ostr;
                ostr << "Line: " << __LINE__ << " Unknown character in object: " << *iter;
                throw runtime_error(ostr.str());
            }
            continue;
        }
        cout << __LINE__ << endl;
        if (flag == false && ret_value_type == STRING_TYPE) {
            iter = this->parser_string(iter, ret_value);
            value_name = ret_value.str_val_;
            flag = true;
            continue;
        }
        cout << __LINE__ << endl;
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
                cout << "string" << endl;
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
            case NUMBER_TYPE:
            {
                iter = this->parser_number(iter, ret_value);
            } break;
        default:
            break;
        }
        if (val.jobject_val_.object_val_.find(value_name) == val.jobject_val_.object_val_.end()) {
            val.jobject_val_[value_name] = ret_value;
        } else {
            ostringstream ostr;
            ostr << "Line: " << __LINE__ << " Array need to surround by []";
            throw runtime_error(ostr.str());
        }
        flag = true;
    }

    ++iter;
    return iter;
}

VALUE_TYPE 
WeJson::check_valuetype(ByteBuffer_Iterator start_pos)
{
    if (isdigit(*start_pos)) {
        return NUMBER_TYPE;
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

ValueTypeCast& 
WeJson::operator[](const string key)
{
    return json_object_.object_val_[key];
}

const ValueTypeCast& 
WeJson::operator[](const string key) const
{
    auto iter = json_object_.object_val_.find(key);
    if (iter == json_object_.object_val_.end()) {
        
        stringstream str_output;
        str_output << "key: " << key << " not exists." << std::endl;
        throw runtime_error(str_output.str());
        return iter->second;
    } else {
        return iter->second;
    }
}

//////////////////////////////////////////////////////////////////

ValueTypeCast& 
JsonObject::operator[](string key) 
{
    if (object_val_.find(key) == object_val_.end()) {
        ostringstream str_output;
        str_output << "key: " << key << " not exists." << std::endl;
        throw runtime_error(str_output.str());
    } else {
        return object_val_[key];
    }
}
const ValueTypeCast& 
JsonObject::operator[](const string key) const 
{
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

bool 
JsonObject::operator==(const JsonObject& rhs) const
{
    if (rhs.object_val_.size() != object_val_.size()) {
        return false;
    }

    for (auto iter = object_val_.begin(); iter != object_val_.end(); ++iter) {
        auto rhs_iter = rhs.object_val_.find(iter->first);
        if (rhs_iter == rhs.object_val_.end()) {
            return false;
        }

        if (rhs_iter->second != iter->second) {
            return false;
        }
    }

    return true;
}

bool 
JsonObject::operator!=(const JsonObject& rhs) const
{
        if (rhs.object_val_.size() != object_val_.size()) {
        return true;
    }

    for (auto iter = object_val_.begin(); iter != object_val_.end(); ++iter) {
        auto rhs_iter = rhs.object_val_.find(iter->first);
        if (rhs_iter == rhs.object_val_.end()) {
            return true;
        }

        if (rhs_iter->second != iter->second) {
            return true;
        }
    }

    return false;
}

ValueTypeCast& 
JsonArray::operator[](size_t key) 
{
    if (key > array_val_.size()) {
        ostringstream str_output;
        str_output << "pos: " << key << " out of range." << std::endl;
        throw runtime_error(str_output.str());
    } else {
        return array_val_[key];
    }
}
const ValueTypeCast& 
JsonArray::operator[](const size_t key) const 
{
    if (key > array_val_.size()) {
        ostringstream str_output;
        str_output << "pos: " << key << " out of range." << std::endl;
        throw runtime_error(str_output.str());
    } else {
        return array_val_[key];
    }
}

bool 
JsonArray::operator==(const JsonArray& rhs) const
{
    if (array_val_.size() != rhs.array_val_.size()) {
        return false;
    }

    for (int i = 0; i < rhs.array_val_.size(); ++i) {
        if (rhs.array_val_[i] != array_val_[i]) {
            return false;
        }
    }

    return true;
}

bool 
JsonArray::operator!=(const JsonArray& rhs) const
{
    if (array_val_.size() != rhs.array_val_.size()) {
        return true;
    }

    for (int i = 0; i < rhs.array_val_.size(); ++i) {
        if (rhs.array_val_[i] != array_val_[i]) {
            return true;
        }
    }

    return false;
}

}