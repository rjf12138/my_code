#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "basic_head.h"
#include "mutex.h"

#define MAX_STRING_SIZE     512
#define MAX_BUFFER_SIZE     1*1024*1024*1024

namespace my_util {

typedef int64_t BUFSIZE_T;
typedef vector<int8_t> BUFFTYPE_T;

class ByteBuffer_Iterator;
class ByteBuffer {
    friend class ByteBuffer_Iterator;
public:
    ByteBuffer(BUFSIZE_T max_buffer_size = 2);
    virtual ~ByteBuffer();

    int read_only_int8(int8_t &val) {}
    int read_only_int16(int16_t &val) {}
    int read_only_int32(int32_t &val) {}
    int read_only_int64(int64_t &val) {}
    int read_only_string(string &str) {}
    BUFSIZE_T read_only_bytes(void *buf, BUFSIZE_T buf_size, bool match = false) {}

    int read_int8(int8_t &val);
    int read_int16(int16_t &val);
    int read_int32(int32_t &val);
    int read_int64(int64_t &val);
    int read_string(string &str);
    BUFSIZE_T read_bytes(void *buf, BUFSIZE_T buf_size, bool match = false);

    int write_int8(int8_t val);
    int write_int16(int16_t val);
    int write_int32(int32_t val);
    int write_int64(int64_t val);
    int write_string(string str);
    BUFSIZE_T write_bytes(const void *buf, BUFSIZE_T buf_size, bool match = false);

    int read_int8_lock(int8_t &val);
    int read_int16_lock(int16_t &val);
    int read_int32_lock(int32_t &val);
    int read_int64_lock(int64_t &val);
    int read_string_lock(string &str);
    BUFSIZE_T read_bytes_lock(void *buf, BUFSIZE_T buf_size, bool match = false);

    int write_int8_lock(int8_t val);
    int write_int16_lock(int16_t val);
    int write_int32_lock(int32_t val);
    int write_int64_lock(int64_t val);
    int write_string_lock(string str);
    BUFSIZE_T write_bytes_lock(const void *buf, BUFSIZE_T buf_size, bool match = false);

    // 网络字节序转换
    // 将缓存中的数据读取出来并转成主机字节序返回
    int read_int16_ntoh(int16_t &val);
    int read_int32_ntoh(int32_t &val);
    // 将主机字节序转成网络字节序写入缓存
    int write_int16_hton(const int16_t &val);
    int write_int32_hton(const int32_t &val);

    bool empty(void);
    int data_size(void);
    int idle_size(void);
    int clear(void);
    // 重新分配缓冲区大小(只能向上增长),minsize表示重新分配缓冲区的下限
    int resize(BUFSIZE_T min_size);

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
    friend ByteBuffer operator+(ByteBuffer &lhs, ByteBuffer &rhs);
    friend bool operator==(ByteBuffer &lhs, ByteBuffer &rhs);
    friend bool operator!=(ByteBuffer &lhs, ByteBuffer &rhs);
    ByteBuffer& operator=(const ByteBuffer& src);

private:
    // 下一个读的位置
    void next_read_pos(int offset = 1);
    // 下一个写的位置
    void next_write_pos(int offset = 1);

    // 将data中的数据拷贝size个字节到当前bytebuff中
    BUFSIZE_T copy_data_to_buffer(const void *data, BUFSIZE_T size);
    // 从bytebuff中拷贝data个字节到data中
    BUFSIZE_T copy_data_from_buffer(void *data, BUFSIZE_T size);

private:
    BUFFTYPE_T buffer_; // 修改为shared_ptr<int8_t>指针来修改
    Mutex lock_;

    BUFSIZE_T start_read_pos_;
    BUFSIZE_T start_write_pos_;

    BUFSIZE_T data_size_;
    BUFSIZE_T max_buffer_size_;

    shared_ptr<ByteBuffer_Iterator> bytebuff_iter_start_;
    shared_ptr<ByteBuffer_Iterator> bytebuff_iter_end_;
};

////////////////////////// ByteBuffer 迭代器 //////////////////////////////////////
class ByteBuffer_Iterator : public iterator<random_access_iterator_tag, int8_t> 
{
    friend class ByteBuffer;
public:
    ByteBuffer_Iterator(void) = default;
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
    ByteBuffer_Iterator& operator+(int inc)
    {
        // 处于end()的迭代器，加多少值都是没用的
        if (curr_pos_ == buff_.start_write_pos_) {
            ostringstream ostr;
            ostr << "Line: " << __LINE__ << "ByteBuffer_Iterator operator+ out of range.";
            throw runtime_error(ostr.str());
            return *this;
        }

        int tmp_pos = (curr_pos_ + buff_.max_buffer_size_ + inc) % buff_.max_buffer_size_;
        if (buff_.start_read_pos_ < buff_.start_write_pos_ && tmp_pos < buff_.start_read_pos_
                        && tmp_pos < buff_.start_write_pos_)
        {
            curr_pos_ = buff_.start_write_pos_;
            ostringstream ostr;
            ostr << "Line: " << __LINE__ << "ByteBuffer_Iterator operator+ out of range.";
            throw runtime_error(ostr.str());
        } else if (buff_.start_read_pos_ < buff_.start_write_pos_ && tmp_pos > buff_.start_read_pos_
                        && tmp_pos >= buff_.start_write_pos_)
        {
            curr_pos_ = buff_.start_write_pos_;
            ostringstream ostr;
            ostr << "Line: " << __LINE__ << "ByteBuffer_Iterator operator+ out of range.";
            throw runtime_error(ostr.str());
        } else if (buff_.start_read_pos_ > buff_.start_write_pos_ && tmp_pos < buff_.start_read_pos_
                        && tmp_pos >= buff_.start_write_pos_)
        {
            curr_pos_ = buff_.start_write_pos_;
            ostringstream ostr;
            ostr << "Line: " << __LINE__ << "ByteBuffer_Iterator operator+ out of range.";
            throw runtime_error(ostr.str());
        }

        curr_pos_ = tmp_pos;
        return *this;
    }
    // 只支持 == ,!= , = 其他的比较都不支持
    bool operator==(const ByteBuffer_Iterator& iter) const {
        return (curr_pos_ == iter.curr_pos_ && buff_ == iter.buff_);
    }
    bool operator!=(const ByteBuffer_Iterator& iter) const {
        return (curr_pos_ != iter.curr_pos_ || buff_ != iter.buff_);
    }
    bool operator>(const ByteBuffer_Iterator& iter) const {
        if (buff_ != iter.buff_) {
            return false;
        }
        if (curr_pos_ > iter.curr_pos_) {
            return true;
        } else if (curr_pos_ < iter.curr_pos_) {
            if (curr_pos_ < buff_.start_read_pos_) {
                return true;
            }
        }

        return false;
    }
    bool operator>=(const ByteBuffer_Iterator& iter) const {
        if (buff_ != iter.buff_) {
            return false;
        }
        if (curr_pos_ >= iter.curr_pos_) {
            return true;
        } else if (curr_pos_ < iter.curr_pos_) {
            if (curr_pos_ < buff_.start_read_pos_) {
                return true;
            }
        }

        return false;
    }
    bool operator<(const ByteBuffer_Iterator& iter) const {
        if (buff_ != iter.buff_) {
            return false;
        }
        if (curr_pos_ >= iter.curr_pos_) {
            return false;
        } else if (curr_pos_ < iter.curr_pos_) {
            if (curr_pos_ < buff_.start_read_pos_) {
                return false;
            }
        }

        return true;
    }
    bool operator<=(const ByteBuffer_Iterator& iter) const {
        if (buff_ != iter.buff_) {
            return false;
        }
        if (curr_pos_ > iter.curr_pos_) {
            return false;
        } else if (curr_pos_ < iter.curr_pos_) {
            if (curr_pos_ < buff_.start_read_pos_) {
                return false;
            }
        }

        return true;
    }
    ByteBuffer_Iterator& operator=(const ByteBuffer_Iterator& src)
    {
        if (src != *this) {
            buff_ = src.buff_;
            curr_pos_ = src.curr_pos_;
        }

        return *this;
    }
    
private:
    ByteBuffer &buff_;
    int32_t curr_pos_;
};

}

#endif