#include "json_type.h"

namespace my_util {

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
JsonNumber::generate(void)
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
    switch (value_type_)
    {
        case DOUBLE_TYPE:
        {
            os << double_value_;
        } break;
        case INT32_TYPE:
        {
            os << int_value_;
        } break;
        default:
        {
            os << "";
        } break;
    }

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
JsonBool::generate(void)
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
JsonNull::generate(void)
{
    return value_;
}

ostream& 
JsonNull::operator<<(ostream &os)
{
    os << this->generate();
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
        if (flag == false && ret_value_type == STRING_TYPE) {
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
            case STRING_TYPE:
            {
                JsonString val;
                iter = val.parse(iter, json_end_pos);
                vtc = val;
            } break;
            case BOOL_TYPE:
            {
                JsonBool val;
                iter = val.parse(iter, json_end_pos);
                vtc = val;
            } break;
            case NULL_TYPE:
            {
                JsonNull val;
                iter = val.parse(iter, json_end_pos);
                vtc = val;
            } break;
            case NUMBER_TYPE:
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
            ostr << "Line: " << __LINE__ << " There's already " << value_name << " exists.";
            throw runtime_error(ostr.str());
        }
        flag = false;
    }

    if (iter != json_end_pos && *iter == '}') {
        ++iter;
    } else {
        ostringstream ostr;
        ostr << "Line: " << __LINE__ << " Object does not end with '}'";
        throw runtime_error(ostr.str());
    }

    return iter;
}

string 
JsonObject::generate(void)
{
    ostringstream output_obj;
    output_obj << "{";
    for (auto iter = object_val_.begin(); iter != object_val_.end(); ++iter) {
        if (iter != object_val_.begin()) {
            output_obj << ",\n\t";
        } else {
            output_obj << "\n\t";
        }
        output_obj << iter->first;
        output_obj << ": ";
        output_obj << iter->second;
    }
    output_obj << "\n}";

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


}