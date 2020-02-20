#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "basic_head.h"
#include "mutex.h"

#define MAX_STRING_SIZE 512

class ByteBuffer_Iterator;
class ByteBuffer {
    friend class ByteBuffer_Iterator;
public:
    ByteBuffer(int max_buffer_size = 64);
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
    /***************************************************** 
    *  使用write_bytes写的是包含类成员的结构时，确保该类成员内不含指针
    *  思考这样一种情况：
    * struct test { string str; } a = {"hello"}, b;
    * buff.write_bytes(&a, sizeof(test));
    * buff.read_bytes(b, sizeof(test))
    * a 和 b 相同, a 和 b 内部的指针指向同一个位置，程序结束时会对同一
    * 个指针释放两次，从而报错double free or corruption (fasttop)
    *******************************************************/
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
    int idle_size(void);
    int clear(void);
    int get_start_pos(void) const {return start_read_pos_;}
    int get_end_pos(void) const {return start_write_pos_;}
    // 重新分配缓冲区大小(只能向上增长)
    int resize(void);

    // 获取错误码，只在错误发生后调用才有效
    int get_error(void);
    // 根据错误码返回错误消息
    string get_err_msg(int err);
    // 获取错消息，只在错误发生后调用才有效
    string get_err_msg(void);
    
    // 返回起始结束迭代器
    ByteBuffer_Iterator begin(void);
    ByteBuffer_Iterator end(void);

    // 重载操作符
    friend ByteBuffer operator+(const ByteBuffer &lhs, const ByteBuffer &rhs);
    friend bool operator==(ByteBuffer &lhs, ByteBuffer &rhs);
    friend bool operator!=(ByteBuffer &lhs, ByteBuffer &rhs);
    ByteBuffer& operator=(const ByteBuffer& src);

private:
    // 下一个读的位置
    void next_read_pos(int offset = 1);
    // 下一个写的位置
    void next_write_pos(int offset = 1);

    // 将data中的数据拷贝size个字节到当前bytebuff中
    int copy_data_to_buffer(const void *data, int size);
    // 从bytebuff中拷贝data个字节到data中
    int copy_data_from_buffer(void *data, int size);

private:
    vector<int8_t> buffer_; // 修改为shared_ptr<int8_t>指针来修改
    Mutex lock_;

    int start_read_pos_;
    int start_write_pos_;

    int incr_size;  // 增加缓存大小时，额外增加的大小
    int data_size_;
    int max_buffer_size_;

    int errno_;

    shared_ptr<ByteBuffer_Iterator> bytebuff_iter_start_;
    shared_ptr<ByteBuffer_Iterator> bytebuff_iter_end_;
};

////////////////////////// ByteBuffer 迭代器 //////////////////////////////////////
class ByteBuffer_Iterator : public iterator<random_access_iterator_tag, int8_t> 
{
    friend class ByteBuffer;
public:
    explicit ByteBuffer_Iterator(ByteBuffer &buff)
            : buff_(buff), curr_pos_(){}

    ByteBuffer_Iterator begin() 
    {
        curr_pos_ = buff_.start_read_pos_;
        return *this;
    }

    ByteBuffer_Iterator end()
    {
        curr_pos_ = buff_.start_write_pos_;
        return *this;
    }

    ByteBuffer_Iterator& operator=(const ByteBuffer_Iterator& src)
    {
        if (src != *this) {
            buff_ = src.buff_;
            curr_pos_ = src.curr_pos_;
        }

        return *this;
    }

    int8_t operator*()
    {
        return buff_.buffer_[curr_pos_];
    }
    // 前置++
    ByteBuffer_Iterator& operator++()
    {
        if (curr_pos_ == buff_.start_write_pos_)
        {
            return *this;
        }
        curr_pos_ = (curr_pos_ + buff_.max_buffer_size_ + 1) % buff_.max_buffer_size_;
        
        return *this;
    }
    // 后置++
    ByteBuffer_Iterator& operator++(int)
    {
        if (curr_pos_ == buff_.start_write_pos_)
        {
            return *this;
        }

        ByteBuffer_Iterator &tmp = *this;
        curr_pos_ = (curr_pos_ + buff_.max_buffer_size_ + 1) % buff_.max_buffer_size_;

        return tmp;
    }
    // 前置--
    ByteBuffer_Iterator& operator--()
    {
        if (curr_pos_ == buff_.start_read_pos_ || curr_pos_ == buff_.start_write_pos_) {
            curr_pos_ = buff_.start_write_pos_;
            return *this;
        }

        curr_pos_ = (curr_pos_ + buff_.max_buffer_size_ - 1) % buff_.max_buffer_size_;

        return *this;
    }
    // 后置--
    ByteBuffer_Iterator& operator--(int)
    {
        if (curr_pos_ == buff_.start_read_pos_ || curr_pos_ == buff_.start_write_pos_) {
            curr_pos_ = buff_.start_write_pos_;
            return *this;
        }

        ByteBuffer_Iterator &tmp = *this;
        curr_pos_ = (curr_pos_ + buff_.max_buffer_size_ - 1) % buff_.max_buffer_size_;

        return tmp;
    }
    // 只支持 == ,!= , = 其他的比较都不支持
    bool operator==(const ByteBuffer_Iterator& iter) const {
        return (curr_pos_ == iter.curr_pos_ && buff_ == iter.buff_);
    }
    bool operator!=(const ByteBuffer_Iterator& iter) const {
        return (curr_pos_ != iter.curr_pos_ || buff_ != iter.buff_);
    }
    
private:
    ByteBuffer &buff_;
    int32_t curr_pos_;
};

#endif