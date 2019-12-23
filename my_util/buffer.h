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

class Buffer {
public:
    Buffer(int max_buffer_size = 1024);
    virtual ~Buffer();

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
    int copy_to_buffer(const Buffer buf, uint32_t start, uint32_t size);

    int8_t* get_buffer(void);
    int get_start_pos(void) const ;
    int get_next_pos(int curr_pos) const;
    int get_prev_pos(int curr_pos) const;
    int get_end_pos(void) const;
    bool empty(void) const;
    bool full(void) const;
    int data_size(void) const;
    int idle_size() const;
    int clear(void);

private:
    void next_read_pos(int offset = 1);
    void next_write_pos(int offset = 1);

    int copy_data_to_buffer(const void *data, int size);
    int copy_data_from_buffer(void *data, int size);

private:
    vector<int8_t> buffer_;
    Mutex lock_;

    int start_read_pos_;
    int start_write_pos_;

    int data_size_;
    int max_buffer_size_;
};

#endif