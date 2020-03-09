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
    for (auto iter = json_buffer_.begin(); iter != json_buffer_.end(); ++iter) {
        for (int i = 0; i < sperate_chars.size(); ++i) {
            if (*iter == sperate_chars[i]) {
                if (*iter == '\\' && *++iter == '"') {
                    continue;
                }
                if (ret_pos.find(*iter) == ret_pos.end()) {
                    vector<ByteBuffer_Iterator> tmp_vec;
                    ret_pos[*iter] = tmp_vec;
                }
                ret_pos[*iter].push_back(iter);
            }
        }
    }

    

    return 0;
}


// ByteBuffer_Iterator 
// WeJson::parser_number(ByteBuffer_Iterator start_pos, value_type &val)
// {
//     ByteBuffer_Iterator iter = start_pos;
//     string str;
//     for (; iter != json_buffer_.end(); ++iter) {
//         if (*iter > '9' || *iter < '0') {
//             break;
//         }
//         str += *iter;
//     }
//     val.type_ = INT_TYPE;
//     val.int_val_ = stoi(str, 0, str.length());
//     return iter;
// }
// ByteBuffer_Iterator 
// WeJson::parser_boolean(ByteBuffer_Iterator start_pos, value_type &val)
// {
//     ByteBuffer_Iterator iter = start_pos;
//     int read_size = 0;
//     if (*iter == 't') {
//         read_size = 4;
//     } else if (*iter == 'f') {
//         read_size = 5;
//     }

//     string str;
//     for (int i = 0; i < read_size; ++i) {
//         str += *iter;
//         ++iter;
//     }

//     val.type_ = BOOL_TYPE;
//     if (str == "true") {
//         val.bool_val_ = true;
//     } else if (str == "false") {
//         val.bool_val_ = false;
//     }

//     return iter;
// }

// ByteBuffer_Iterator 
// WeJson::parser_string(ByteBuffer_Iterator start_pos, value_type &val)
// {
//     ByteBuffer_Iterator iter = start_pos;
//     ++iter; // 此时 *iter 值为 "

//     string str;
//     for (; *iter != '"' || iter != json_buffer_.end(); ++iter) {
//         str += *iter;
//     }

//     if (iter == json_buffer_.end())
//     {
//         // TODO
//     }

//     val.type_ = STRING_TYPE;
//     val.str_val_ = this->escape_string(str);
//     ++iter; // 此时 iter 值为 "

//     return iter;
// }

// ByteBuffer_Iterator 
// WeJson::parser_array(ByteBuffer_Iterator start_pos, value_type &val)
// {
//     val.type_ = JSON_ARRAY_TYPE;
//     ByteBuffer_Iterator iter = start_pos, val_head_pos;
//     ++iter;

//     for ()
// }

// ByteBuffer_Iterator WeJson::parser_object(ByteBuffer_Iterator start_pos, value_type &val);

// void parser_from_json(ByteBuffer &buff)
// {

// }
// void parser_from_json(string str);
// void generate_to_json(string &str);

// ValueTypeCast& operator[](const string key);
// const ValueTypeCast& operator[](const string key) const;
// }
//