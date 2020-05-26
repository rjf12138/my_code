#ifndef __JSON_TYPE_H__
#define __JSON_TYPE_H__

#include "basic_head.h"
#include "byte_buffer.h"
#include "msg_record.h"

namespace my_util {
enum VALUE_TYPE {
    UNKNOWN_TYPE = -1000,
    JSON_NUMBER_TYPE = 10001,
    JSON_BOOL_TYPE = 10002,
    JSON_NULL_TYPE = 10003,
    JSON_STRING_TYPE = 10004,
    JSON_ARRAY_TYPE = 10005,
    JSON_OBJECT_TYPE = 10006
};

enum NUMBER_TYPE_ {
    NONE_TYPE = -1001,
    INT32_TYPE = 10030,
    DOUBLE_TYPE = 10031,
};

class JsonType : public MsgRecord {
public:
    // 检查当前位置的字符来判断接下来的是什么类型，具体参考doc中的资料
    static VALUE_TYPE check_value_type(ByteBuffer_Iterator &iter);
    // 解析遇到的类型，具体取决于check_value_type()返回的类型和继承该类的实现
    virtual ByteBuffer_Iterator parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos){}
    // 将当前类型以字符串方式输出，ctrl_ch控制输出的格式，为了方便阅读
    virtual string generate(string ctrl_ch = "\0") {};
    // 获取以value_curr_pos为中心，range为半径范围的json值，主要为了调试用
    string get_json_text(ByteBuffer_Iterator &value_curr_pos, int range);
    // 输出调试信息
    virtual string debug_info(ByteBuffer_Iterator &value_curr_pos);
private:
    VALUE_TYPE json_value_type_;
};

// json 数值类型
class JsonNumber : public JsonType{
public:
    JsonNumber(void);
    JsonNumber(double val);
    JsonNumber(int32_t val);
    JsonNumber(const JsonNumber& val);
    ~JsonNumber(void);

    virtual ByteBuffer_Iterator parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos) override;
    virtual string generate(string ctrl_ch = "\0") override;

    ostream& operator<<(ostream &os);
    bool operator==(const JsonNumber& rhs) const;
    bool operator!=(const JsonNumber& rhs) const;
    JsonNumber& operator=(JsonNumber rhs);
    operator double();
    operator int();

private:
    NUMBER_TYPE_  value_type_;
    double        double_value_;
    int32_t       int_value_;
};

// json 布尔类型
class JsonBool : public JsonType {
public:
    JsonBool(void);
    JsonBool(bool val);
    ~JsonBool(void);

    virtual ByteBuffer_Iterator parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos) override;
    virtual string generate(string ctrl_ch = "\0") override;

    ostream& operator<<(ostream &os);
    bool operator==(const JsonBool& rhs) const;
    bool operator!=(const JsonBool& rhs) const;
    JsonBool& operator=(JsonBool rhs);
    operator bool();

private:
    bool value_;
};

// json null 类型
class JsonNull : public JsonType {
public:
    JsonNull(void);
    JsonNull(string val);
    ~JsonNull(void);

    virtual ByteBuffer_Iterator parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos) override;
    virtual string generate(string ctrl_ch = "\0") override;

    ostream& operator<<(ostream &os);
    bool operator==(const JsonNull& rhs) const;
    bool operator!=(const JsonNull& rhs) const;
    JsonNull& operator=(JsonNull rhs);
    operator string();

private:
    string value_;
};

// json 字符串类型
class JsonString : public JsonType {
public:
    JsonString(void);
    JsonString(string val);
    ~JsonString(void);

    virtual ByteBuffer_Iterator parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos) override;
    virtual string generate(string ctrl_ch = "\0") override;

    ostream& operator<<(ostream &os);
    bool operator==(const JsonString& rhs) const;
    bool operator!=(const JsonString& rhs) const;
    JsonString& operator=(JsonString rhs);
    operator string();

private:
    string value_;
};

// json 对象类型
class ValueTypeCast;
class JsonObject : public JsonType {
public:
    JsonObject(void);
    ~JsonObject(void);

    virtual ByteBuffer_Iterator parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos) override;
    virtual string generate(string ctrl_ch = "\0");

    ostream& operator<<(ostream &os);
    bool operator==(const JsonObject& rhs) const;
    bool operator!=(const JsonObject& rhs) const;
    JsonObject& operator=(JsonObject rhs);
    ValueTypeCast& operator[](string key);

private:
    map<string, ValueTypeCast> object_val_;
};

// json 数组类型
class JsonArray : public JsonType {
public:
    JsonArray(void);
    ~JsonArray(void);

    virtual ByteBuffer_Iterator parse(ByteBuffer_Iterator &value_start_pos, ByteBuffer_Iterator &json_end_pos) override;
    virtual string generate(string ctrl_ch = "\0");

    ostream& operator<<(ostream &os);
    ValueTypeCast& operator[](size_t key);
    const ValueTypeCast& operator[](const size_t key) const;
    bool operator==(const JsonArray& rhs) const;
    bool operator!=(const JsonArray& rhs) const; 
    JsonArray& operator=(JsonArray rhs);

public:
    vector<ValueTypeCast> array_val_;
};

// json中转类型：可以安装当前存储的类型输出或是接收不同的类型
class ValueTypeCast : public JsonType {
public:
    ValueTypeCast(void);
    ValueTypeCast(JsonBool value);
    ValueTypeCast(JsonNumber value);
    ValueTypeCast(JsonString value);
    ValueTypeCast(JsonObject value);
    ValueTypeCast(JsonArray value);
    ValueTypeCast(JsonNull value);
    ValueTypeCast(const ValueTypeCast& value); // 只能使用引用不然会报错
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
    ostringstream& operator<<(ostringstream &ostr);
    bool operator==(const ValueTypeCast& rhs) const;
    bool operator!=(const ValueTypeCast& rhs) const;

    string generate(string ctrl_ch = "\0");

public:
    VALUE_TYPE json_value_type_;
    JsonArray json_array_value_;
    JsonObject json_object_value_;
    JsonString json_string_value_;
    JsonNumber json_number_value_;
    JsonBool   json_bool_value_;
    JsonNull json_null_value_;
};

}

#endif