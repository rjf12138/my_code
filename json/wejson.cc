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

    return 0;
}

int 
WeJson::write_json(string json_file_path)
{
    FileOperate json_file;
    if (json_file.open(json_file_path) == -1) {
        ostringstream ostr;
        ostr << "Line: " << __LINE__ << " file: " << json_file_path << " is not exists.";
        throw runtime_error(ostr.str());
    }

    ByteBuffer buff;
    buff.write_bytes(this->generate_to_json().c_str(), this->generate_to_json().length());
    json_file.write(buff, buff.data_size());

    return 0;
}

int 
WeJson::parser_from_json(string str)
{
    raw_json_buffer_.clear();
    raw_json_buffer_.write_bytes(str.c_str(), str.length());
    return this->parser_from_json(raw_json_buffer_);
}

int 
WeJson::parser_from_json(ByteBuffer &buff)
{
    bool quotation_marks = false;
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
    auto begin_json = simplify_json_buffer_.begin();
    auto end_json = simplify_json_buffer_.end();
    VALUE_TYPE ret_type = JsonType::check_value_type(begin_json);
    if (ret_type == JSON_ARRAY_TYPE) {
        json_value_.json_array_value_.parse(begin_json, end_json);
        json_value_.json_value_type_ = JSON_ARRAY_TYPE;
    } else if (ret_type == JSON_OBJECT_TYPE) {
        json_value_.json_object_value_.parse(begin_json, end_json);
        json_value_.json_value_type_ = JSON_OBJECT_TYPE;
    } else {
        ostringstream ostr;
        ostr << "Line: " << __LINE__ << " Error json text!";
        throw runtime_error(ostr.str());
    }

    return 0;
}

ValueTypeCast& 
WeJson::operator[](string key)
{
    if (json_value_.json_value_type_ != JSON_OBJECT_TYPE) {
        ostringstream ostr;
        ostr << "Line: " << __LINE__ << " Can't visit json value!";
        throw runtime_error(ostr.str());
    }

    return json_value_.json_object_value_[key];
}
ValueTypeCast& 
WeJson::operator[](int key)
{
    if (json_value_.json_value_type_ != JSON_OBJECT_TYPE) {
        ostringstream ostr;
        ostr << "Line: " << __LINE__ << " Can't visit json value!";
        throw runtime_error(ostr.str());
    }

    return json_value_.json_array_value_[key];
}

string 
WeJson::generate_to_json(void)
{
    return json_value_.generate();
}

}