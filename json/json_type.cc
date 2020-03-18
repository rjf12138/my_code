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

    bool is_decimal = false;
    for (; iter != json_end_pos && *iter != ','; ++iter) {
        if (*iter == '0' && iter == value_start_pos) {
            if (isdigit(*(iter+1))) { // 除了小数和数值0之外，零不能作为第一个数
                ostringstream ostr;
                ostr << "Line: " << __LINE__ << " Zero can't be first number of integer!";
                throw runtime_error(ostr.str());
            }
        }

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
        double_value_ = stod(str);
    } else {
        value_type_ = INT32_TYPE;
        double_value_ = stoi(str);
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

virtual bool operator==(const JsonNumber& rhs) const;
virtual bool operator!=(const JsonNumber& rhs) const;
operator double();
operator int();

}