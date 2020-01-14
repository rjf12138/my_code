/*
*   start_pos 指向第一个可读/写位置， end_pos 指向最后一个可读写之后的位置
*   整个buffer采用循环队列的方式保存数据
*   可能还要考虑高低字节的顺序，目前先不考虑
*   需要让buff自己增大空间的功能
*/

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "basic_head.h"
#include "mutex.h"

#define MAX_STRING_SIZE 512

class ByteBuffer_Iterator;
class ByteBuffer {
    friend class ByteBuffer_Iterator;
public:
    ByteBuffer(int max_buffer_size = 1024);
    virtual ~ByteBuffer();

    int read_int8(int8_t &val);
    int read_int16(int16_t &val);
    int read_int32(int32_t &val);
    int read_int64(int64_t &val);
    int read_string(string &str);
    int read_bytes(void *buf, int buf_size, bool match = false);

    int write_int8(int8_t val);
    int write_int16(int16_t val);
    int write_int32(int32_t val);
    int write_int64(int64_t val);
    int write_string(string str);
    int write_bytes(const void *buf, int buf_size, bool match = false);

    int read_int8_lock(int8_t &val);
    int read_int16_lock(int16_t &val);
    int read_int32_lock(int32_t &val);
    int read_int64_lock(int64_t &val);
    int read_string_lock(string &str);
    int read_bytes_lock(void *buf, int buf_size, bool match = false);

    int write_int8_lock(int8_t val);
    int write_int16_lock(int16_t val);
    int write_int32_lock(int32_t val);
    int write_int64_lock(int64_t val);
    int write_string_lock(string str);
    int write_bytes_lock(const void *buf, int buf_size, bool match = false);

    // 拷贝从 start 起 size 个字节， start 是指从 start_read_pos_ 起的字节数
    int copy_to_buffer(const ByteBuffer buf, uint32_t start, uint32_t size);

    // 网络字节序转换
    // 将缓存中的数据读取出来并转成主机字节序返回
    int read_int16_ntoh(int16_t &val);
    int read_int32_ntoh(int32_t &val);
    // 将数据转成字节序写入缓存
    int write_int16_ntoh(const int16_t &val);
    int write_int32_ntoh(const int32_t &val);

    bool empty(void);
    bool full(void);
    int data_size(void);
    int idle_size();
    int clear(void);


    // 获取错误码，只在错误发生后调用才有效
    int get_error(void);
    // 根据错误码返回错误消息
    string get_err_msg(int err);
    // 获取错消息，只在错误发生后调用才有效
    string get_err_msg(void);

    // 重载操作符
    friend ByteBuffer operator+(const ByteBuffer &lhs, const ByteBuffer &rhs);
    friend bool operator==(const ByteBuffer &lhs, const ByteBuffer &rhs);
    
private:
    // 下一个读的位置
    void next_read_pos(int offset = 1);
    // 下一个写的位置
    void next_write_pos(int offset = 1);

    // 将data中的数据拷贝size个字节到当前bytebuff中
    int copy_data_to_buffer(const void *data, int size);
    // 从bytebuff中拷贝data个字节到data中
    int copy_data_from_buffer(void *data, int size);

    // 获取buff缓冲指针， 用于不修改内部读写位置而进行的预览
    int8_t* get_buffer(void);
    // 获取起始位置
    int get_start_pos(void);
    // 根据当前位置， 获取下一个字节位置
    int get_next_pos(int curr_pos);
    // 根据当前位置， 回到上一个读取位置
    int get_prev_pos(int curr_pos);
    // 获取结束位置
    int get_end_pos(void);

private:
    vector<int8_t> buffer_;
    Mutex lock_;

    int start_read_pos_;
    int start_write_pos_;

    int data_size_;
    int max_buffer_size_;

    int errno_;

    ByteBuffer_Iterator bytebuff_iterator_;
};

// 查看当数据变动是迭代器是否有影响

// ByteBuffer 迭代器
class ByteBuffer_Iterator : public iterator<random_access_iterator_tag, int8_t> 
{
    friend class ByteBuffer;
public:
    explicit ByteBuffer_Iterator(ByteBuffer &buff)
            : buff_(buff), curr_pos_(){}

    ByteBuffer_Iterator& operator=(const ByteBuffer_Iterator& src)
    {
        buff_ = src.buff_;
        curr_pos_ = src.curr_pos_;
    }

    int8_t& operator*()
    {
        return buff_.get_buffer()[curr_pos_];
    }

    ByteBuffer_Iterator* operator++()
    {
        if (curr_pos_ == buff_.get_end_pos())
        {
            return this;
        }
        curr_pos_ = buff_.get_next_pos(curr_pos_);
        return this;
    }

    ByteBuffer_Iterator* operator--()
    {
        if (curr_pos_ == buff_.get_start_pos())
        {
            return this;
        }
        curr_pos_ = buff_.get_prev_pos(curr_pos_);
        return this;
    }

    bool operator<(const ByteBuffer_Iterator& iter) const {return this->curr_pos_ < iter.curr_pos_;}
    bool operator==(const ByteBuffer_Iterator& iter) const {return this->curr_pos_ == iter.curr_pos_;}
    bool operator!=(const ByteBuffer_Iterator& iter) const {return this->curr_pos_ != iter.curr_pos_;}
    bool operator>(const ByteBuffer_Iterator& iter) const {return this->curr_pos_ > iter.curr_pos_;}
    bool operator<=(const ByteBuffer_Iterator& iter) const {return this->curr_pos_ <= iter.curr_pos_;}
    bool operator>=(const ByteBuffer_Iterator& iter) const {return this->curr_pos_ >= iter.curr_pos_;}

private:
    ByteBuffer &buff_;
    int8_t curr_pos_;
};

#endif