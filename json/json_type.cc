#include "json_type.h"

namespace my_util {

VALUE_TYPE
JsonType::check_value_type(ByteBuffer_Iterator &iter) 
{
        if (isdigit(*iter) || *iter == '-' || *iter == '+') {
            return JSON_NUMBER_TYPE;
        }
        switch (*iter)
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
                return JSON_STRING_TYPE;
            } break;
            case 't':
            {
                return JSON_BOOL_TYPE;
            }break;
            case 'f':
            {
                return JSON_BOOL_TYPE;
            }break;
            case 'n':
            {
                return JSON_NULL_TYPE;
            } break;
            default:
                break;
        }

        return UNKNOWN_TYPE;
    }

///////////////////////////////////////////////////////////

JsonNumber::JsonNumber(void)
: int_value_(0), double_value_(0), value_type_(NONE_TYPE)
{

}
JsonNumber::JsonNumber(double val)
: value_type_(DOUBLE_TYPE), double_value_(val)
{

}
JsonNumber::JsonNumber(int32_t val)
: value_type_(INT32_TYPE), int_value_(val)
{

}

JsonNumber::JsonNumber(const JsonNumber& val)
: value_type_(val.value_type_), int_value_(val.int_value_), double_value_(val.double_value_)
{

}

JsonNumber::~JsonNumber(void) {}

ByteBuffer_Iterator 
JsonNumber::parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos)
{
    ByteBuffer_Iterator iter = value_start_pos;
    string str;
    bool is_negative = false;

    if (*iter == '-') {
        is_negative = true;
        ++iter;
    } else if (*iter == '+') {
        is_negative = false;
        ++iter;
    }

    if (iter != json_end_pos && *iter == '0') {
        if (isdigit(*(iter+1))) { // 除了小数和数值0之外，零不能作为第一个数
            ostringstream ostr;
            ostr << "Line: " << __LINE__ << " Zero can't be first number of integer!";
            throw runtime_error(ostr.str());
        }
    }

    bool is_decimal = false;
    for (; iter != json_end_pos && *iter != ','; ++iter) {
        if (!isdigit(*iter) && *iter != '.') {
            break;
        }

        if (*iter == '.') {
            is_decimal = true;
        }
        str += *iter;
    }
    if (is_decimal) {
        value_type_ = DOUBLE_TYPE;
        double_value_ = stod(str) * (is_negative?-1:1);
    } else {
        value_type_ = INT32_TYPE;
        int_value_ = stoi(str) * (is_negative?-1:1);
    }

    return iter;
}

string 
JsonNumber::generate(string ctrl_ch)
{
    switch (value_type_)
    {
    case DOUBLE_TYPE:
    {
        return to_string(double_value_);
    } break;
    case INT32_TYPE:
    {
        return to_string(int_value_);
    } break;
    default:
        break;
    }

    return string("");
}

ostream& 
JsonNumber::operator<<(ostream &os)
{
    os << this->generate();

    return os;
}

bool
JsonNumber::operator==(const JsonNumber& rhs) const
{
    if (value_type_ == rhs.value_type_) {
        if (value_type_ == INT32_TYPE) {
            return int_value_ == rhs.int_value_?true:false;
        } else if (value_type_ == DOUBLE_TYPE) {
            return double_value_ == rhs.double_value_?true:false;
        }
    }

    return false;
}
bool 
JsonNumber::operator!=(const JsonNumber& rhs) const
{
    return !(*this == rhs);
}

JsonNumber::operator double()
{
    if (value_type_ == INT32_TYPE) {
        return int_value_;
    } else if (value_type_ == DOUBLE_TYPE) {
        return double_value_;
    }
}

JsonNumber::operator int()
{
    if (value_type_ == INT32_TYPE) {
        return int_value_;
    } else if (value_type_ == DOUBLE_TYPE) {
        return static_cast<int>(double_value_);
    }
}

JsonNumber& 
JsonNumber::operator=(JsonNumber rhs)
{
    value_type_ = rhs.value_type_;
    if (value_type_ == INT32_TYPE) {
        int_value_ = rhs.int_value_;
    } else if (value_type_ == DOUBLE_TYPE) {
        double_value_ = rhs.double_value_;
    }

    return *this;
}

/////////////////////////////////////////////////////////////////

JsonBool::JsonBool(void)
{

}

JsonBool::JsonBool(bool val)
{
    value_ = val;
}
JsonBool::~JsonBool(void)
{

}

ByteBuffer_Iterator 
JsonBool::parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos)
{
    ByteBuffer_Iterator iter = value_start_pos;
    int read_size = 0;
    if (*iter == 't') {
        read_size = 4;
    } else if (*iter == 'f') {
        read_size = 5;
    }

    string str;
    for (int i = 0; iter != json_end_pos && i < read_size; ++iter, ++i) {
        str += *iter;
    }
    
    if (str == "true") {
        value_ = true;
    } else if (str == "false") {
        value_ = false;
    } else {
        ostringstream ostr;
        ostr << "Line: " << __LINE__ << " Expected true or false, but result is " << str;
        throw runtime_error(ostr.str());
    }

    return iter;
}

string 
JsonBool::generate(string ctrl_ch)
{
    return value_ == true? "true":"false";
}

ostream& 
JsonBool::operator<<(ostream &os)
{
    os << this->generate();

    return os;
}

bool 
JsonBool::operator==(const JsonBool& rhs) const
{
    return value_ == rhs.value_?true:false;
}

bool 
JsonBool::operator!=(const JsonBool& rhs) const
{
    return !(*this == rhs);
}
JsonBool::operator bool()
{
    return value_;
}

JsonBool& 
JsonBool::operator=(JsonBool rhs)
{
    value_ = rhs.value_;

    return *this;
}


//////////////////////////////////////////////////////////////
JsonNull::JsonNull(void)
{

}
JsonNull::JsonNull(string val)
{
    value_ = val;
}
JsonNull::~JsonNull(void)
{}

ByteBuffer_Iterator 
JsonNull::parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos)
{
    ByteBuffer_Iterator iter = value_start_pos;
    int read_size = 4; // null is four bytes.

    string str;
    for (int i = 0; iter != json_end_pos && i < read_size; ++iter, ++i) {
        str += *iter;
    }

    if (str == "null") {
        value_ = str;
    } else {
        ostringstream ostr;
        ostr << "Line: " << __LINE__ << " Expected null, but result is " << str;
        throw runtime_error(ostr.str());
    }


    return iter;
}

string 
JsonNull::generate(string ctrl_ch)
{
    return value_;
}

ostream& 
JsonNull::operator<<(ostream &os)
{
    os << this->generate();

    return os;
}

bool 
JsonNull::operator==(const JsonNull& rhs) const
{
    return (value_ == "null" ? true : false);
}

bool 
JsonNull::operator!=(const JsonNull& rhs) const
{
    return !(*this == rhs);
}

JsonNull::operator string()
{
    return value_;
}

JsonNull& 
JsonNull::operator=(JsonNull rhs)
{
    value_ = rhs.value_;
}


///////////////////////////////////////////////////////////

JsonString::JsonString(void) {}
JsonString::JsonString(string val): value_(val) {}
JsonString::~JsonString(void) {}

ByteBuffer_Iterator 
JsonString::parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos)
{
    ByteBuffer_Iterator iter = value_start_pos;
    ++iter; // 此时 *iter 值为 ",指向字符串的第一个字符
    string str;
    for (; iter != json_end_pos; ++iter) {
        if (*iter == '"' && *(iter - 1) != '\\') {
            break;
        }
        str += *iter;
    }

    if (iter == json_end_pos)
    {
        ostringstream ostr;
        ostr << "Line: " << __LINE__ << " String need to surround by \"\" " << str;
        throw runtime_error(ostr.str());
    }
    value_ = str;
    ++iter; //  iter 指向下一个字符

    return iter;
}

string 
JsonString::generate(string ctrl_ch)
{
    string str = "\"";
    str += value_ + "\"";
    return str;
}

ostream& 
JsonString::operator<<(ostream &os)
{
    os << this->generate();

    return os;
}

bool JsonString::operator==(const JsonString& rhs) const
{
    if (value_ != rhs.value_) {
        return false;
    }

    return true;
}

bool JsonString::operator!=(const JsonString& rhs) const
{
    return !(*this == rhs);
}

JsonString::operator string()
{
    return value_;
}

JsonString& 
JsonString::operator=(JsonString rhs)
{
    value_ = rhs.value_;

    return *this;
}

///////////////////////////////////////////////////////////

JsonObject::JsonObject(void) {}
JsonObject::~JsonObject(void) {}

ByteBuffer_Iterator 
JsonObject::parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos)
{
    ByteBuffer_Iterator iter = value_start_pos;
    ++iter; // 由指向'{'改到指向第一个键值对

    string value_name;
    bool flag = false;
    for (; iter != json_end_pos && *iter != '}'; ++iter) {
        VALUE_TYPE ret_value_type = this->check_value_type(iter);
        if (ret_value_type == UNKNOWN_TYPE) {
            if (*iter != ',' && *iter != ':' && *iter != ']') {
                ostringstream ostr;
                ostr << "Line: " << __LINE__ << " Unknown character in object: " << *iter;
                throw runtime_error(ostr.str());
            }
            continue;
        }

        JsonString json_str;
        if (flag == false && ret_value_type == JSON_STRING_TYPE) {
            iter = json_str.parse(iter, json_end_pos);
            value_name = json_str;
            flag = true;
            continue;
        }

        ValueTypeCast vtc;
        switch (ret_value_type)
        {
            case JSON_OBJECT_TYPE:
            {
                JsonObject val;
                iter = val.parse(iter, json_end_pos);
                vtc = val;
            } break;
            case JSON_ARRAY_TYPE:
            {
                JsonArray val;
                iter = val.parse(iter, json_end_pos);
                vtc = val;
            } break;
            case JSON_STRING_TYPE:
            {
                JsonString val;
                iter = val.parse(iter, json_end_pos);
                vtc = val;
            } break;
            case JSON_BOOL_TYPE:
            {
                JsonBool val;
                iter = val.parse(iter, json_end_pos);
                vtc = val;
            } break;
            case JSON_NULL_TYPE:
            {
                JsonNull val;
                iter = val.parse(iter, json_end_pos);
                vtc = val;
            } break;
            case JSON_NUMBER_TYPE:
            {
                JsonNumber val;
                iter = val.parse(iter, json_end_pos);
                vtc = val;
            } break;
        default:
            break;
        }

        if (object_val_.find(value_name) == object_val_.end()) {
            object_val_[value_name] = vtc;
        } else {
            ostringstream ostr;
            ostr << "Line: " << __LINE__ << " There's already \"" << value_name << "\" exists.";
            throw runtime_error(ostr.str());
        }
        flag = false;
        if (*iter == '}') { // 有些解析玩就直接指向'}'， 如果不退出在回到循环会因值自增错过
            break;
        }
    }

    if (iter != json_end_pos && *iter == '}') {
        ++iter;
    }
    
    return iter;
}

string 
JsonObject::generate(string ctrl_ch)
{
    ostringstream output_obj;
    output_obj << "{";
    for (auto iter = object_val_.begin(); iter != object_val_.end(); ++iter) {
        if (iter != object_val_.begin()) {
            output_obj << ",";
        }

        output_obj << "\n\t" << ctrl_ch << iter->first;
        output_obj << ": ";
        if (iter->second.json_value_type_ == JSON_ARRAY_TYPE ||
                iter->second.json_value_type_ == JSON_OBJECT_TYPE) {
            string next_ctrl = ctrl_ch + "\t";
            output_obj << (iter->second).generate(next_ctrl);
        } else {
            output_obj << (iter->second).generate();
        }
    }
    output_obj << "\n" << ctrl_ch << "}";

    return output_obj.str();
}

ostream& 
JsonObject::operator<<(ostream &os)
{
    os << this->generate();

    return os;
}

bool 
JsonObject::operator==(const JsonObject& rhs) const
{
    if (this->object_val_.size() != rhs.object_val_.size()) {
        return false;
    }

    auto iter_this = this->object_val_.begin();
    auto iter_rhs = rhs.object_val_.begin();
    while (true) {
        if (iter_rhs == rhs.object_val_.end() && iter_this == this->object_val_.end()) {
            return true;
        }

        if (iter_this->first != iter_rhs->first) {
            return false;
        }

        if (iter_this->second != iter_rhs->second) {
            return false;
        }
    }

    return false;
}

bool 
JsonObject::operator!=(const JsonObject& rhs) const
{
    return !(*this == rhs);
}

ValueTypeCast& 
JsonObject::operator[](string key)
{
    return object_val_[key];
}

JsonObject& 
JsonObject::operator=(JsonObject rhs)
{
    object_val_ = rhs.object_val_;

    return *this;
}

////////////////////////////////////////////////////////////

JsonArray::JsonArray(void){}
JsonArray::~JsonArray(void){}

ByteBuffer_Iterator 
JsonArray::parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos)
{
    ByteBuffer_Iterator iter = value_start_pos;
    ++iter;

    ValueTypeCast ret_value;
    for (; iter != json_end_pos && *iter != ']'; ++iter) {
        VALUE_TYPE ret_value_type = this->check_value_type(iter);
        if (ret_value_type == UNKNOWN_TYPE) {
            if (*iter != ',') {
                ostringstream ostr;
                ostr << "Line: " << __LINE__ << " Unknown character in array: " << *iter;
                throw runtime_error(ostr.str());
            }
            continue;
        }
        ValueTypeCast vtc;
        switch (ret_value_type)
        {
            case JSON_OBJECT_TYPE:
            {
                JsonObject val;
                iter = val.parse(iter, json_end_pos);
                vtc = val;
            } break;
            case JSON_ARRAY_TYPE:
            {
                JsonArray val;
                iter = val.parse(iter, json_end_pos);
                vtc = val;
            } break;
            case JSON_STRING_TYPE:
            {
                JsonString val;
                iter = val.parse(iter, json_end_pos);
                vtc = val;
            } break;
            case JSON_BOOL_TYPE:
            {
                JsonBool val;
                iter = val.parse(iter, json_end_pos);
                vtc = val;
            } break;
            case JSON_NULL_TYPE:
            {
                JsonNull val;
                iter = val.parse(iter, json_end_pos);
                vtc = val;
            } break;
            case JSON_NUMBER_TYPE:
            {
                JsonNumber val;
                iter = val.parse(iter, json_end_pos);
                vtc = val;
            } break;
        default:
            break;
        }
        
        array_val_.push_back(vtc);
        if (*iter == ']') { // 有些解析玩就直接指向'】'， 如果不退出在回到循环会因值自增错过
            break;
        }
    }
    if (iter == json_end_pos) {
        ostringstream ostr;
        ostr << "Line: " << __LINE__ << " Array need to surround by []";
        throw runtime_error(ostr.str());
    }
    ++iter;
    return iter;
}
string 
JsonArray::generate(string ctrl_ch)
{
    ostringstream ostr;
    ostr << "[";
    for (int i = 0; i < array_val_.size(); ++i) {
        if (i != 0) { // 每输出一个类型后跟一个','
            ostr << ",\n" << ctrl_ch;
        } else {
            ostr << "\n" << ctrl_ch;
        }
        if (array_val_[i].json_value_type_ == JSON_ARRAY_TYPE ||
                array_val_[i].json_value_type_ == JSON_OBJECT_TYPE) {
            string next_ctrl = ctrl_ch + "\t";
            ostr << ctrl_ch << array_val_[i].generate(next_ctrl);
        } else {
            ostr << ctrl_ch << array_val_[i].generate();
        }
    }

    ostr << "\n" << ctrl_ch << "]";
    return ostr.str();
}

ostream& 
JsonArray::operator<<(ostream &os)
{
    os << this->generate();

    return os;
}

ValueTypeCast& 
JsonArray::operator[](size_t key)
{
    return array_val_[key];
}

bool 
JsonArray::operator==(const JsonArray& rhs) const
{
    if (array_val_.size() != rhs.array_val_.size()) {
        return false;
    }

    for (int i = 0; i > array_val_.size(); ++i) {
        if (array_val_[i].json_array_value_ != rhs.array_val_[i].json_array_value_) {
            return false;
        }
        if (array_val_[i] != rhs.array_val_[i]) {
            return false;
        }
    }

    return true;
}
bool JsonArray::operator!=(const JsonArray& rhs) const
{
    return !(*this == rhs);
}

JsonArray& 
JsonArray::operator=(JsonArray rhs)
{
    array_val_ = rhs.array_val_;

    return *this;
}

/////////////////////////////////////////////////////////////////

ValueTypeCast::ValueTypeCast(void) {}

ValueTypeCast::ValueTypeCast(JsonBool value)
    : json_value_type_(JSON_BOOL_TYPE), json_bool_value_(value) {}
    
ValueTypeCast::ValueTypeCast(JsonNumber value)
    : json_number_value_(value), json_value_type_(JSON_NUMBER_TYPE) {}

ValueTypeCast::ValueTypeCast(JsonString value)
    : json_string_value_(value), json_value_type_(JSON_STRING_TYPE) {}

ValueTypeCast::ValueTypeCast(JsonObject value)
    :json_object_value_(value), json_value_type_(JSON_OBJECT_TYPE) {}

ValueTypeCast::ValueTypeCast(JsonArray value)
    : json_array_value_(value), json_value_type_(JSON_ARRAY_TYPE) {}

ValueTypeCast::ValueTypeCast(JsonNull value)
    : json_null_value_(value), json_value_type_(JSON_NULL_TYPE) {}

ValueTypeCast::ValueTypeCast(const ValueTypeCast &value)
    : json_value_type_(value.json_value_type_),
      json_array_value_(value.json_array_value_),
      json_bool_value_(value.json_bool_value_),
      json_null_value_(value.json_null_value_),
      json_number_value_(value.json_number_value_),
      json_string_value_(value.json_string_value_),
      json_object_value_(value.json_object_value_)
{}

ValueTypeCast::~ValueTypeCast(void) {}

ValueTypeCast::operator JsonBool()
{
    if (json_value_type_ == JSON_BOOL_TYPE) {
        return json_bool_value_;
    } else {
        throw runtime_error("value cast faled: current type is not bool");
    }
}

ValueTypeCast::operator JsonNumber()
{
    if (json_value_type_ == JSON_NUMBER_TYPE) {
        return json_number_value_;
    } else {
        throw runtime_error("value cast faled: current type is not number");
    }
}
ValueTypeCast::operator JsonString()
{
    if (json_value_type_ == JSON_STRING_TYPE) {
        return json_string_value_;
    } else {
        throw runtime_error("value cast faled: current type is not string");
    }
}

ValueTypeCast::operator JsonObject()
{
    if (json_value_type_ == JSON_OBJECT_TYPE) {
        return json_object_value_;
    } else {
        throw runtime_error("value cast faled: current type is not object");
    }
}
ValueTypeCast::operator JsonArray()
{
    if (json_value_type_ == JSON_OBJECT_TYPE) {
        return json_array_value_;
    } else {
        throw runtime_error("value cast faled: current type is not array");
    }
}

ValueTypeCast::operator JsonNull()
{
    if (json_value_type_ == JSON_NULL_TYPE) {
        return json_null_value_;
    } else {
        throw runtime_error("value cast failed: current type is not null");
    }
}

ValueTypeCast& 
ValueTypeCast::operator=(JsonBool val)
{
    json_value_type_ = JSON_BOOL_TYPE;
    json_bool_value_ = val;

    return *this;
}

ValueTypeCast& 
ValueTypeCast::operator=(JsonNumber val)
{
    json_value_type_ = JSON_NUMBER_TYPE;
    json_number_value_ = val;

    return *this;
}

ValueTypeCast& 
ValueTypeCast::operator=(JsonString val)
{
    json_value_type_ = JSON_STRING_TYPE;
    json_string_value_ = val;

    return *this;
}

ValueTypeCast& 
ValueTypeCast::operator=(JsonObject val)
{
    json_value_type_ = JSON_OBJECT_TYPE;
    json_object_value_ = val;

    return *this;
}

ValueTypeCast& 
ValueTypeCast::operator=(JsonArray val)
{
    json_value_type_ = JSON_ARRAY_TYPE;
    json_array_value_ = val;

    return *this;
}

ValueTypeCast& 
ValueTypeCast::operator=(JsonNull val)
{
    json_value_type_ = JSON_NULL_TYPE;
    json_null_value_ = val;

    return *this;
}

ValueTypeCast& 
ValueTypeCast::operator=(ValueTypeCast val)
{
    json_value_type_ = val.json_value_type_;
    json_bool_value_ = val.json_bool_value_;
    json_null_value_ = val.json_null_value_;
    json_array_value_ = val.json_array_value_;
    json_object_value_ = val.json_object_value_;
    json_string_value_ = val.json_string_value_;
    json_number_value_ = val.json_number_value_;

    return *this;
}

ostream& 
ValueTypeCast::operator<<(ostream &os)
{
    switch (json_value_type_)
    {
    case JSON_NULL_TYPE:
        os << json_null_value_.generate();
        break;
    case JSON_NUMBER_TYPE:
        os << json_number_value_.generate();
        break;
    case JSON_STRING_TYPE:
        os << json_string_value_.generate();
        break;
    case JSON_BOOL_TYPE:
        os << json_bool_value_.generate();
        break;
    case JSON_ARRAY_TYPE:
        os << json_array_value_.generate();
        break;
    case  JSON_OBJECT_TYPE:
        os << json_object_value_.generate();
        break;
    default:
        os << "NONE_TYPE";
        break;
    }

    return os;
}

bool ValueTypeCast::operator==(const ValueTypeCast& rhs) const
{
    switch (json_value_type_)
    {
    case JSON_NULL_TYPE:
        return json_null_value_ == rhs.json_null_value_;
    case JSON_NUMBER_TYPE:
        return json_number_value_ == rhs.json_number_value_;
    case JSON_STRING_TYPE:
        return json_string_value_ == rhs.json_string_value_;
    case JSON_BOOL_TYPE:
        return json_bool_value_ == rhs.json_bool_value_;
    case JSON_ARRAY_TYPE:
        return json_array_value_ == rhs.json_array_value_;
    case  JSON_OBJECT_TYPE:
        return json_object_value_ == rhs.json_object_value_;
    default:
        break;
    }

    return true;
}
bool ValueTypeCast::operator!=(const ValueTypeCast& rhs) const
{
    return !(*this == rhs);
}

string ValueTypeCast::generate(string ctrl_ch)
{
    switch (json_value_type_)
    {
    case JSON_NULL_TYPE:
        return json_null_value_.generate(ctrl_ch);
    case JSON_NUMBER_TYPE:
        return json_number_value_.generate(ctrl_ch);
    case JSON_STRING_TYPE:
        return json_string_value_.generate(ctrl_ch);
    case JSON_BOOL_TYPE:
        return json_bool_value_.generate(ctrl_ch);
    case JSON_ARRAY_TYPE:
        return json_array_value_.generate(ctrl_ch);
    case  JSON_OBJECT_TYPE:
        return json_object_value_.generate(ctrl_ch);
    default:
        break;
    }

    return "";
}

}