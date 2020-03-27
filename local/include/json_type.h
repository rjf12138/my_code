#ifndef __JSON_TYPE_H__
#define __JSON_TYPE_H__

#include "basic_head.h"
#include "byte_buffer.h"

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

enum NUMBER_TYPE_ {
    NONE_TYPE = -1001,
    INT32_TYPE = 10030,
    DOUBLE_TYPE = 10031,
};

class JsonType {
public:
    virtual ByteBuffer_Iterator parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos){}
    virtual string generate(void) {};

    virtual ostream& operator<<(ostream &os) {};
    virtual istream& operator>>(istream &is) {};
    virtual VALUE_TYPE check_value_type(ByteBuffer_Iterator &iter) {
        if (isdigit(*iter) || *iter == '-' || *iter == '+') {
            return NUMBER_TYPE;
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
private:
    VALUE_TYPE json_value_type_;
};

class JsonNumber : public JsonType{
public:
    JsonNumber(void);
    JsonNumber(double val);
    JsonNumber(int32_t val);
    ~JsonNumber(void);

    virtual ByteBuffer_Iterator parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos) override;
    virtual string generate(void) override;

    virtual ostream& operator<<(ostream &os) override;
    virtual bool operator==(const JsonNumber& rhs) const;
    virtual bool operator!=(const JsonNumber& rhs) const;
    virtual JsonNumber& operator=(JsonNumber rhs);
    operator double();
    operator int();

private:
    NUMBER_TYPE_  value_type_;
    double        double_value_;
    int32_t       int_value_;
};

class JsonBool : public JsonType {
public:
    JsonBool(void);
    JsonBool(bool val);
    ~JsonBool(void);

    virtual ByteBuffer_Iterator parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos) override;
    virtual string generate(void) override;

    virtual ostream& operator<<(ostream &os) override;
    virtual bool operator==(const JsonBool& rhs) const;
    virtual bool operator!=(const JsonBool& rhs) const;
    virtual JsonBool& operator=(JsonBool rhs);
    operator bool();

private:
    bool value_;
};

class JsonNull : public JsonType {
public:
    JsonNull(void);
    JsonNull(string val);
    ~JsonNull(void);

    virtual ByteBuffer_Iterator parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos) override;
    virtual string generate(void) override;

    virtual ostream& operator<<(ostream &os) override;
    // // virtual istream& operator>>(istream &is) override;
    virtual bool operator==(const JsonNull& rhs) const;
    virtual bool operator!=(const JsonNull& rhs) const;
    virtual JsonNull& operator=(JsonNull rhs);
    operator string();

private:
    string value_;
};

class JsonString : public JsonType {
public:
    JsonString(void);
    JsonString(string val);
    ~JsonString(void);

    virtual ByteBuffer_Iterator parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos) override;
    virtual string generate(void) override;

    virtual ostream& operator<<(ostream &os) override;
    // virtual istream& operator>>(istream &is) override;
    virtual bool operator==(const JsonString& rhs) const;
    virtual bool operator!=(const JsonString& rhs) const;
    virtual JsonString& operator=(JsonString rhs);
    operator string();

private:
    string value_;
};

class ValueTypeCast;
class JsonObject : public JsonType {
    JsonObject(void);
    ~JsonObject(void);

    virtual ByteBuffer_Iterator parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos) override;
    virtual string generate(void) override;

    virtual ostream& operator<<(ostream &os) override;
    virtual bool operator==(const JsonObject& rhs) const;
    virtual bool operator!=(const JsonObject& rhs) const;
    virtual JsonObject& operator=(JsonObject rhs);
    ValueTypeCast& operator[](string key);

private:
    map<string, ValueTypeCast> object_val_;
};

class JsonArray : public JsonType {
public:
    JsonArray(void);
    ~JsonArray(void);

    virtual ByteBuffer_Iterator parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos) override;
    virtual string generate(void) override;

    virtual ostream& operator<<(ostream &os) override;
    ValueTypeCast& operator[](size_t key);
    const ValueTypeCast& operator[](const size_t key) const;
    bool operator==(const JsonArray& rhs) const;
    bool operator!=(const JsonArray& rhs) const; 
    virtual JsonArray& operator=(JsonArray rhs);

public:
    vector<ValueTypeCast> array_val_;
};

class ValueTypeCast {
public:
    ValueTypeCast(void);
    ValueTypeCast(JsonBool value);
    ValueTypeCast(JsonNumber value);
    ValueTypeCast(JsonString value);
    ValueTypeCast(JsonObject value);
    ValueTypeCast(JsonArray value);
    ValueTypeCast(JsonNull value);
    ~ValueTypeCast(void);

    operator JsonBool();
    operator JsonNumber();
    operator JsonString();
    operator JsonObject();
    operator JsonArray();
    operator JsonNull();

    ValueTypeCast& operator=(JsonBool val);
    ValueTypeCast& operator=(JsonNumber val);
    ValueTypeCast& operator=(JsonString val);
    ValueTypeCast& operator=(JsonObject val);
    ValueTypeCast& operator=(JsonArray val);
    ValueTypeCast& operator=(JsonNull val);
    ValueTypeCast& operator=(ValueTypeCast val);

    ostream& operator<<(ostream &os);
    bool operator==(const ValueTypeCast& rhs) const;
    bool operator!=(const ValueTypeCast& rhs) const;

public:
    VALUE_TYPE json_value_type_;
    JsonArray json_array_value_;
    JsonObject json_object_value_;
    JsonString json_string_value_;
    JsonNumber json_number_value_;
    JsonBool   json_bool_value_;
    JsonNull json_null_value_;
};

// class ValueTypeCast {
// public:
//     ValueTypeCast(void){}
//     ValueTypeCast(double val): type_(NUMBER_TYPE), number_val_(val){}
//     ValueTypeCast(int32_t val): type_(NUMBER_TYPE), number_val_(val){}
//     ValueTypeCast(string val): type_(STRING_TYPE), str_val_(val){}
//     ValueTypeCast(bool val): type_(BOOL_TYPE), bool_val_(val){}
//     ValueTypeCast(int* val): type_(NULL_TYPE), null_val_(val){}
//     ValueTypeCast(JsonObject val): type_(JSON_OBJECT_TYPE), jobject_val_(val){}
//     ValueTypeCast(JsonArray val): type_(JSON_ARRAY_TYPE), jarray_val_(val){}
//     ~ValueTypeCast() = default;

//     operator double()  {
//         if (type_ == NUMBER_TYPE) {
//             return number_val_;
//         }
//     }
//     operator bool()  {
//         if (type_ == BOOL_TYPE) {
//             return bool_val_;
//         }
//     }
//     operator string()   {
//         if (type_ == STRING_TYPE) {
//             return str_val_;
//         }
//     }
//     operator JsonArray()   {
//         if (type_ == JSON_ARRAY_TYPE) {
//             return jarray_val_;
//         }
//     }
//     operator JsonObject()  {
//         if (type_ == JSON_OBJECT_TYPE) {
//             return jobject_val_;
//         }
//     }

//     ValueTypeCast& operator=(double val) {
//         type_ = NUMBER_TYPE;
//         number_val_ = val;

//         return *this;
//     }

//     ValueTypeCast& operator=(string val) {
//         type_ = STRING_TYPE;
//         str_val_ = val;
//     }

//     ValueTypeCast& operator=(bool val) {
//         type_ = BOOL_TYPE;
//         bool_val_ = val;
//         return *this;
//     }

//     ValueTypeCast& operator=(int* val) {
//         type_ = NULL_TYPE;
//         null_val_ = val;
//         return *this;
//     }

//     ValueTypeCast& operator=(JsonObject val) {
//         type_ = JSON_OBJECT_TYPE;
//         jobject_val_ = val;
//         return *this;
//     }

//     ValueTypeCast& operator=(JsonArray val) {
//         type_ = JSON_ARRAY_TYPE;
//         jarray_val_ = val;
//         return *this;
//     }

//     bool operator==(const double& rhs) {
//         if (type_ != NUMBER_TYPE) {
//             return false;
//         }

//         if (rhs != number_val_) {
//             return false;
//         }

//         return true;
//     }

//     bool operator==(const string& rhs) {
//         if (type_ != STRING_TYPE) {
//             return false;
//         }

//         if (rhs != str_val_) {
//             return false;
//         }

//         return true;
//     }

//     bool operator==(const bool& rhs) {
//         if (type_ != BOOL_TYPE) {
//             return false;
//         }

//         if (rhs != bool_val_) {
//             return false;
//         }

//         return true;
//     }

//     bool operator==(const int*& rhs) {
//         if (type_ != NULL_TYPE) {
//             return false;
//         }

//         if (rhs != null_val_) {
//             return false;
//         }

//         return true;
//     }

//     bool operator==(const JsonObject& rhs) {
//         if (type_ != JSON_OBJECT_TYPE) {
//             return false;
//         }

//         if (rhs != jobject_val_) {
//             return false;
//         }

//         return true;
//     }

//     bool operator==(const JsonArray& rhs) {
//         if (type_ != JSON_ARRAY_TYPE) {
//             return false;
//         }

//         if (rhs != jarray_val_) {
//             return false;
//         }

//         return true;
//     }

//     bool operator==(const ValueTypeCast& rhs) const {
//         if (type_ != rhs.type_) {
//             return false;
//         }

//         switch (type_)
//         {
//             case JSON_OBJECT_TYPE:
//             {
//                 return (jobject_val_ == rhs.jobject_val_);
//             } break;
//             case JSON_ARRAY_TYPE:
//             {
//                 return (jarray_val_ == rhs.jarray_val_);
//             } break;
//             case STRING_TYPE:
//             {
//                 return (str_val_ == rhs.str_val_);
//             } break;
//             case BOOL_TYPE:
//             {
//                 return (bool_val_ == rhs.bool_val_);
//             }break;
//             case NULL_TYPE:
//             {
//                 return (null_val_ == rhs.null_val_);
//             } break;
//             case NUMBER_TYPE:
//             {
//                 return (number_val_ == rhs.number_val_);
//             } break;
//         default:
//             break;
//         }

//         return false;
//     }

//     bool operator!=(const ValueTypeCast& rhs) const {
//         return !(*this == rhs);
//     }
// public :
//     VALUE_TYPE type_;
//     double number_val_;
//     bool bool_val_;
//     string str_val_;
//     int *null_val_;
//     JsonArray jarray_val_;
//     JsonObject jobject_val_;
// };

}

#endif