#include "buffer.h"

Buffer::Buffer(int max_buffer_size = 1024)
{
    max_buffer_size_ = max_buffer_size;
    buffer_.reserve(max_buffer_size_);
    this->clear();
}

Buffer::~Buffer()
{
    this->clear();
}

int8_t*
Buffer::get_buffer(void)
{
    return buffer_.data;
}

int Buffer::clear(void)
{
    data_size_ = 0;
    start_read_pos_ = 0;
    start_write_pos_ = 0;
}

int Buffer::get_start_pos(void) const 
{
    return start_read_pos_;
}
int Buffer::get_next_pos(int curr_pos) const
{
    return (curr_pos == start_write_pos_) ? start_write_pos_ : (curr_pos + 1) % max_buffer_size_;
}

int Buffer::get_prev_pos(int curr_pos) const
{
    return (curr_pos == start_read_pos_) ? start_read_pos_ : (curr_pos + max_buffer_size_ - 1) % max_buffer_size_;
}

int Buffer::get_end_pos(void) const 
{
    return start_write_pos_;
}

void Buffer::next_read_pos(int offset = 1)
{
    start_read_pos_ = (start_read_pos_ + offset) % max_buffer_size_;
}

void Buffer::next_write_pos(int offset = 1)
{
    start_write_pos_ = (start_write_pos_ + offset) % max_buffer_size_;
}

int Buffer::data_size(void) const 
{
    return data_size_;
}

int Buffer::idle_size() const 
{
    return (max_buffer_size_ - data_size_);
}

bool Buffer::empty(void) const 
{
    return this->data_size() == 0 ? true : false;
}

bool Buffer::full(void) const 
{
    return this->idle_size() == 0 ? true : false;
}

int Buffer::copy_data_to_buffer(const void *data, int size)
{
    if (this->idle_size() < size) {
        return 0;
    }

    int8_t *data_ptr = (int8_t*)data;
    int8_t *ptr = buffer_.data;
    // 检查buff数组后面是否有连续的内存可以写
    int remain = max_buffer_size_ - start_write_pos_;
    if (remain >= size) {    // 有足够的空间，那直接拷贝
        memcpy(ptr+start_write_pos_, data_ptr, size);
        this->next_write_pos(size);
    } else {
        memcpy(ptr+start_write_pos_, data_ptr, remain);
        this->next_write_pos(remain); // 将buff最后的空间写满
        memcpy(ptr, data_ptr + remain, size - remain);
        this->next_write_pos(size - remain); // 从buff开头在将剩余的数据写入
    }

    data_size_ += size; // 更新buff内的数据个数
    return size;
}

int Buffer::copy_to_buffer(Buffer buf, uint32_t start, uint32_t size)
{
    if (size > this->data_size() - start) {
        return ERROR_OUT_OF_RANGE;
    }

    int read_pos = start + buf.get_start_pos();
    int8_t *data = buf.get_buffer();
    for (int i = 0; i < size; ++i) {
        this->write_int8(data[read_pos]);
        read_pos = buf.get_next_pos(read_pos);
    }

    return size;
}

int Buffer::copy_data_from_buffer(void *data, int size)
{
    if (this->data_size() < size) {
        return 0;
    }

    int8_t *data_ptr = (int8_t*)data;
    int8_t *ptr = buffer_.data;
    // 检查buff数组后面是否有连续的内存可以读
    int end_point = start_read_pos_ > start_write_pos_?max_buffer_size_:start_write_pos_;
    int remain = end_point - start_read_pos_;
    if (remain >= size) {    // 有足够的空间，那直接拷贝
        memcpy(data_ptr, ptr+start_read_pos_, size);
        this->next_read_pos(size);
    } else {
        memcpy(data_ptr, ptr+start_read_pos_, remain);
        this->next_read_pos(remain); // 将buff最后的空间读取
        memcpy(data_ptr, ptr, size - remain);
        this->next_read_pos(size - remain); // 从buff开头在将剩余的数据读取
    }

    data_size_ -= size; // 更新buff内的数据个数
    return size;
}

int 
Buffer::read_int8(int8_t &val)
{
    return this->copy_data_from_buffer(&val, sizeof(int8_t));
}

int Buffer::read_int16(int16_t &val)
{
    return this->copy_data_from_buffer(&val, sizeof(int16_t));
}

int Buffer::read_int32(int32_t &val)
{
    return this->copy_data_from_buffer(&val, sizeof(int32_t));
}

int Buffer::read_int64(int64_t &val)
{
    return this->copy_data_from_buffer(&val, sizeof(int64_t));
}

// 字符串是以 ‘\0’ 结尾的
int Buffer::read_string(string &str)
{
    if (this->empty()) {
        return 0;
    }

    // 读取字符串最后的位置, 如果没读到'\0'，那读取长度为 0
    int str_size = 0;
    for (int i = start_read_pos_ ; i < start_write_pos_; ++i) {
        if (buffer_[i] == '\0') {
            str_size = i - start_read_pos_ + 1; // 加一是为了将'\0'这个字符也能读出来
            break;
        }
    }

    char buf[MAX_STRING_SIZE] = {0};
    this->copy_data_from_buffer(buf, str_size);
    str = buf;

    return str_size - 1; // 计算字符串长度时，‘\0’ 不计入
}

int Buffer::read_bytes(void *buf, int buf_size, bool match = false)
{
    if (buf == NULL) {
        return 0;
    }

    if (this->data_size() < buf_size ) { 
        if (match == false) {
            buf_size = this->data_size();
        } else {
            return 0;
        }
    }

    return this->copy_data_from_buffer(buf, buf_size);
}

int Buffer::write_int8(int8_t val)
{
    return this->copy_data_to_buffer(&val, sizeof(int8_t));
}

int Buffer::write_int16(int16_t val)
{
    return this->copy_data_to_buffer(&val, sizeof(int16_t));
}
int Buffer::write_int32(int32_t val)
{
    return this->copy_data_to_buffer(&val, sizeof(int32_t));
}

int Buffer::write_int64(int64_t val)
{
    return this->copy_data_to_buffer(&val, sizeof(int64_t));
}

int Buffer::write_string(string str)
{
    char buf[MAX_STRING_SIZE] = { 0 };
    snprintf(buf, MAX_STRING_SIZE - 1, "%s", str.c_str());

    return this->copy_data_to_buffer(buf, strlen(buf) + 1); // 加1是为了加个'\0'字符
}
int Buffer::write_bytes(const void *buf, int buf_size, bool match = false)
{
    if (buf == NULL) {
        return 0;
    }

    return this->copy_data_to_buffer(buf, buf_size);
}

int Buffer::read_int8_lock(int8_t &val)
{
    lock_.lock();
    int ret_size = this->read_int8(val);
    lock_.unlock();

    return ret_size;
}
int Buffer::read_int16_lock(int16_t &val)
{
    lock_.lock();
    int ret_size = this->read_int16(val);
    lock_.unlock();

    return ret_size;
}
int Buffer::read_int32_lock(int32_t &val)
{
    lock_.lock();
    int ret_size = this->read_int32(val);
    lock_.unlock();

    return ret_size;
}

int Buffer::read_int64_lock(int64_t &val)
{
    lock_.lock();
    int ret_size = this->read_int64(val);
    lock_.unlock();

    return ret_size;
}

int Buffer::read_string_lock(string &str)
{
    lock_.lock();
    int ret_size = this->read_string(str);
    lock_.unlock();

    return ret_size;
}

int Buffer::read_bytes_lock(void *buf, int buf_size, bool match = false)
{
    lock_.lock();
    int ret_size = this->read_bytes(buf, buf_size, match);
    lock_.unlock();

    return ret_size;
}

int Buffer::write_int8_lock(int8_t val)
{
    lock_.lock();
    int ret_size = this->write_int8(val);
    lock_.unlock();

    return ret_size;
}

int Buffer::write_int16_lock(int16_t val)
{
    lock_.lock();
    int ret_size = this->write_int16(val);
    lock_.unlock();

    return ret_size;
}

int Buffer::write_int32_lock(int32_t val)
{
    lock_.lock();
    int ret_size = this->write_int32(val);
    lock_.unlock();

    return ret_size;
}
int Buffer::write_int64_lock(int64_t val)
{
    lock_.lock();
    int ret_size = this->write_int64(val);
    lock_.unlock();

    return ret_size;
}
int Buffer::write_string_lock(string str)
{
    lock_.lock();
    int ret_size = this->write_string(str);
    lock_.unlock();

    return ret_size;
}
int Buffer::write_bytes_lock(const void *buf, int buf_size, bool match = false)
{
    lock_.lock();
    int ret_size = this->write_bytes(buf, buf_size, match);
    lock_.unlock();

    return ret_size;
}