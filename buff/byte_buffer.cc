#include "byte_buffer.h"

namespace my_util {

ByteBuffer::ByteBuffer(BUFSIZE_T max_buffer_size)
{
    max_buffer_size_ = max_buffer_size;
    this->clear();
    this->resize(max_buffer_size);
}

ByteBuffer::~ByteBuffer()
{
    this->clear();
}

int ByteBuffer::clear(void)
{
    errno_ = BYTE_BUFF_SUCCESS;
    data_size_ = 0;
    start_read_pos_ = 0;
    start_write_pos_ = 0;

    return 0;
}

void ByteBuffer::next_read_pos(int offset)
{
    errno_ = BYTE_BUFF_SUCCESS;
    start_read_pos_ = (start_read_pos_ + offset) % max_buffer_size_;
}

void ByteBuffer::next_write_pos(int offset)
{
    errno_ = BYTE_BUFF_SUCCESS;
    start_write_pos_ = (start_write_pos_ + offset) % max_buffer_size_;
}

int ByteBuffer::data_size(void)
{
    errno_ = BYTE_BUFF_SUCCESS;
    return data_size_;
}

int ByteBuffer::idle_size()
{
    errno_ = BYTE_BUFF_SUCCESS;
    return (max_buffer_size_ - data_size_);
}

int 
ByteBuffer::resize(BUFSIZE_T min_size)
{
    BUFSIZE_T new_size = max_buffer_size_;
    while (new_size <= min_size) {
        if (new_size <= 0) {
            new_size = 1;
        } else {
            new_size = new_size * 2;
        }
    }

    BUFFTYPE_T tmp_buf;
    for (auto iter = this->begin(); iter != this->end(); ++iter) {
        tmp_buf.push_back(*iter);
    }

    buffer_.reserve(new_size);
    max_buffer_size_ = buffer_.capacity();
    this->clear();
    for (auto iter = tmp_buf.begin(); iter != tmp_buf.end(); ++iter) {
        this->write_int8(*iter);
    }

    return max_buffer_size_;
}


bool ByteBuffer::empty(void)
{
    errno_ = BYTE_BUFF_SUCCESS;
    return this->data_size() == 0 ? true : false;
}

ByteBuffer_Iterator
ByteBuffer::begin(void)
{
    ByteBuffer_Iterator tmp(*this);
    return tmp.begin();
}

ByteBuffer_Iterator
ByteBuffer::end(void)
{
    ByteBuffer_Iterator tmp(*this);
    return tmp.end();
}

BUFSIZE_T ByteBuffer::copy_data_to_buffer(const void *data, BUFSIZE_T size)
{
    if (data == nullptr) {
        errno_ = BYTE_BUFF_OUTPUT_BUFFER_IS_NULL;
        return -1;
    }

    if (this->idle_size() <= size) {
       this->resize(max_buffer_size_ + size);
    }

    int8_t *data_ptr = (int8_t*)data;
    int8_t *ptr = (int8_t*)buffer_.data();
    // 检查buff数组后面是否有连续的内存可以写
    BUFSIZE_T remain = max_buffer_size_ - start_write_pos_;
    if (remain >= size) {    // 有足够的空间，那直接拷贝
        memcpy(ptr+start_write_pos_, data_ptr, size);
        this->next_write_pos(size);
    } else {
        memcpy(ptr+start_write_pos_, data_ptr, remain);
        this->next_write_pos(remain); // 将buff最后的空间写满
        memcpy(ptr+start_write_pos_, data_ptr + remain, size - remain);
        this->next_write_pos(size - remain); // 从buff开头在将剩余的数据写入
    }

    data_size_ += size; // 更新buff内的数据个数
    errno_ = BYTE_BUFF_SUCCESS;

    return size;
}

BUFSIZE_T ByteBuffer::copy_data_from_buffer(void *data, BUFSIZE_T size)
{
    if (data == nullptr) {
        errno_ = BYTE_BUFF_OUTPUT_BUFFER_IS_NULL;
        return -1;
    }
   
    if (this->data_size() < size) {
        errno_ = BYTE_BUFF_REMAIN_DATA_NOT_ENOUGH;
        return -1;
    }

    int8_t *data_ptr = (int8_t*)data;
    int8_t *ptr = (int8_t*)buffer_.data();
 
    // 检查buff数组后面是否有连续的内存可以读
    BUFSIZE_T end_point = start_read_pos_>start_write_pos_?max_buffer_size_:start_write_pos_;
    BUFSIZE_T remain = end_point - start_read_pos_;
    if (remain >= size) {    // 有足够的空间，那直接拷贝
        memcpy(data_ptr, ptr + start_read_pos_, size);
        this->next_read_pos(size);
    } else {
        memcpy(data_ptr, ptr + start_read_pos_, remain);
        this->next_read_pos(remain); // 将buff最后的空间读取
        memcpy(data_ptr + remain, ptr, size - remain);
        this->next_read_pos(size - remain); // 从buff开头在将剩余的数据读取
    }

    errno_ = BYTE_BUFF_SUCCESS;
    data_size_ -= size; // 更新buff内的数据个数
    return size;
}

int 
ByteBuffer::read_int8(int8_t &val)
{
    return this->copy_data_from_buffer(&val, sizeof(int8_t));
}

int ByteBuffer::read_int16(int16_t &val)
{
    return this->copy_data_from_buffer(&val, sizeof(int16_t));
}

int ByteBuffer::read_int32(int32_t &val)
{
    return this->copy_data_from_buffer(&val, sizeof(int32_t));
}

int ByteBuffer::read_int64(int64_t &val)
{
    return this->copy_data_from_buffer(&val, sizeof(BUFSIZE_T));
}

// 字符串是以 ‘\0’ 结尾的
int ByteBuffer::read_string(string &str)
{
    if (this->empty()) {
        errno_ = BYTE_BUFF_EMPTY;
        return -1;
    }

    // 读取字符串的长度
    int str_size = 0;
    auto iter = this->begin();
    for (; iter != this->end(); ++iter) {
        str_size++;
        if (*iter == '\0') {
            break;
        }
    }
    if (iter == this->end()) {
        errno_ = BYTE_BUFF_CANT_FIND_STRING;
        return -1;
    }

    char buf[MAX_STRING_SIZE] = {0};
    if (this->copy_data_from_buffer(buf, str_size) == -1) {
        return -1;
    }
    str = buf;

    return str_size - 1; // 计算字符串长度时，‘\0’ 不计入
}

BUFSIZE_T ByteBuffer::read_bytes(void *buf, BUFSIZE_T buf_size, bool match)
{
    if (buf == nullptr) {
        errno_ = BYTE_BUFF_OUTPUT_BUFFER_IS_NULL;
        return -1;
    }

    return this->copy_data_from_buffer(buf, buf_size);
}

int ByteBuffer::write_int8(int8_t val)
{
    return this->copy_data_to_buffer(&val, sizeof(int8_t));
}

int ByteBuffer::write_int16(int16_t val)
{
    return this->copy_data_to_buffer(&val, sizeof(int16_t));
}

int ByteBuffer::write_int32(int32_t val)
{
    return this->copy_data_to_buffer(&val, sizeof(int32_t));
}

int ByteBuffer::write_int64(int64_t val)
{
    return this->copy_data_to_buffer(&val, sizeof(BUFSIZE_T));
}

int ByteBuffer::write_string(string str)
{
    // 加1是为了加个'\0'字符, -1 是为了返回长度时去掉'\0'
    return this->copy_data_to_buffer(str.c_str(), str.length() + 1) -1;
}

BUFSIZE_T ByteBuffer::write_bytes(const void *buf, BUFSIZE_T buf_size, bool match)
{
    if (buf == NULL) {
        errno_ = BYTE_BUFF_OUTPUT_BUFFER_IS_NULL;
        return -1;
    }

    return this->copy_data_to_buffer(buf, buf_size);
}

int ByteBuffer::read_int8_lock(int8_t &val)
{
    lock_.lock();
    int ret_size = this->read_int8(val);
    lock_.unlock();

    return ret_size;
}

int ByteBuffer::read_int16_lock(int16_t &val)
{
    lock_.lock();
    int ret_size = this->read_int16(val);
    lock_.unlock();

    return ret_size;
}

int ByteBuffer::read_int32_lock(int32_t &val)
{
    lock_.lock();
    int ret_size = this->read_int32(val);
    lock_.unlock();

    return ret_size;
}

int ByteBuffer::read_int64_lock(int64_t &val)
{
    lock_.lock();
    int ret_size = this->read_int64(val);
    lock_.unlock();

    return ret_size;
}

int ByteBuffer::read_string_lock(string &str)
{
    lock_.lock();
    int ret_size = this->read_string(str);
    lock_.unlock();

    return ret_size;
}

BUFSIZE_T ByteBuffer::read_bytes_lock(void *buf, BUFSIZE_T buf_size, bool match)
{
    lock_.lock();
    int ret_size = this->read_bytes(buf, buf_size, match);
    lock_.unlock();

    return ret_size;
}

int ByteBuffer::write_int8_lock(int8_t val)
{
    lock_.lock();
    int ret_size = this->write_int8(val);
    lock_.unlock();

    return ret_size;
}

int ByteBuffer::write_int16_lock(int16_t val)
{
    lock_.lock();
    int ret_size = this->write_int16(val);
    lock_.unlock();

    return ret_size;
}

int ByteBuffer::write_int32_lock(int32_t val)
{
    lock_.lock();
    int ret_size = this->write_int32(val);
    lock_.unlock();

    return ret_size;
}
int ByteBuffer::write_int64_lock(int64_t val)
{
    lock_.lock();
    int ret_size = this->write_int64(val);
    lock_.unlock();

    return ret_size;
}
int ByteBuffer::write_string_lock(string str)
{
    lock_.lock();
    int ret_size = this->write_string(str);
    lock_.unlock();

    return ret_size;
}
BUFSIZE_T ByteBuffer::write_bytes_lock(const void *buf, BUFSIZE_T buf_size, bool match)
{
    lock_.lock();
    int ret_size = this->write_bytes(buf, buf_size, match);
    lock_.unlock();

    return ret_size;
}

int ByteBuffer::read_int16_ntoh(int16_t &val)
{
    int ret = this->read_int16(val);
    if (ret == -1) {
        return -1;
    }
    val = ntohs(val);

    return 0;
}
int ByteBuffer::read_int32_ntoh(int32_t &val)
{
    int ret = this->read_int32(val);
    if (ret == -1) {
        return -1;
    }
    val = ntohl(val);

    return 0;
}

int ByteBuffer::write_int16_hton(const int16_t &val)
{
    int16_t tmp = val;
    tmp = htons(val);
    int ret = this->write_int16(tmp);

    return ret;
}

int ByteBuffer::write_int32_hton(const int32_t &val)
{
    int32_t tmp = val;
    tmp = htonl(val);
    int  ret = this->write_int32(tmp);

    return ret;
}

int 
ByteBuffer::get_error(void)
{
    return errno_;
}

string 
ByteBuffer::get_err_msg(void)
{
    return this->get_err_msg(errno_);
}

string 
ByteBuffer::get_err_msg(int err)
{
    string error_msg;
    switch(errno_) {
        case BYTE_BUFF_SUCCESS: 
            error_msg = STR_BYTE_BUFF_SUCCESS;
            break;
        case BYTE_BUFF_EMPTY:
            error_msg = STR_BYTE_BUFF_EMPTY;
            break;
        case BYTE_BUFF_FULL:
            error_msg = STR_BYTE_BUFF_FULL;
            break;
        case BYTE_BUFF_STR_READ_FAILED:
            error_msg = STR_BYTE_BUFF_STR_READ_FAILED;
            break;
        case BYTE_BUFF_OUTPUT_BUFFER_IS_NULL:
            error_msg = STR_BYTE_BUFF_OUTPUT_BUFFER_IS_NULL;
            break;
        case BYTE_BUFF_OUT_OF_RANGE:
            error_msg = STR_BYTE_BUFF_OUT_OF_RANGE;
            break;
        case BYTE_BUFF_REMAIN_DATA_NOT_ENOUGH:
            error_msg = STR_BYTE_BUFF_REMAIN_DATA_NOT_ENOUGH;
            break;
        case BYTE_BUFF_CANT_FIND_STRING:
            error_msg = STR_BYTE_BUFF_CANT_FIND_STRING;
        case BYTE_BUFF_RUN_OUT_OF_MEMORY:
            error_msg = STR_BYTE_RUN_OUT_OF_MEMORY;
        default:
            error_msg = STR_UNKNOWN_ERROR;
    }

    return error_msg;
}

//////////////////////// 重载操作符 /////////////////////////

ByteBuffer 
operator+(ByteBuffer &lhs, ByteBuffer &rhs)
{
    // +10 为了提高冗余；
    ByteBuffer out(lhs.data_size() + rhs.data_size()+10);

    for (auto iter = lhs.begin(); iter != lhs.end(); ++iter) {
        out.write_int8(*iter);
    }
    for (auto iter = rhs.begin(); iter != rhs.end(); ++iter) {
        out.write_int8(*iter);
    }

    return out;
}

bool 
operator==(ByteBuffer &lhs, ByteBuffer &rhs)
{
    auto lhs_iter = lhs.begin();
    auto rhs_iter = rhs.begin();

    while (true) {
        if (lhs_iter == lhs.end() && rhs_iter == rhs.end()) {
            return true;
        } else if (lhs_iter != lhs.end() && rhs_iter == rhs.end()) {
            return false;
        } if (lhs_iter == lhs.end() && rhs_iter != rhs.end()) {
            return false;
        }

        if (*lhs_iter != *rhs_iter) {
            return false;
        }

        lhs_iter++;
        rhs_iter++;
    }
}

bool 
operator!=(ByteBuffer &lhs, ByteBuffer &rhs)
{
    if (lhs.data_size() == rhs.data_size()) {
        return false;
    }
    auto lhs_iter = lhs.begin();
    auto rhs_iter = rhs.begin();

    while (true) {
        if (lhs_iter == lhs.end() && rhs_iter == rhs.end()) {
            return false;
        }

        if (*lhs_iter != *rhs_iter) {
            return true;
        }

        lhs_iter++;
        rhs_iter++;
    }
    return false;
}

ByteBuffer& 
ByteBuffer::operator=(const ByteBuffer& src)
{
    buffer_ = src.buffer_;
    lock_ = src.lock_;
    start_read_pos_ = src.start_read_pos_;
    start_write_pos_ = src.start_write_pos_;
    data_size_ = src.data_size_;
    max_buffer_size_ = src.max_buffer_size_;
    errno_ = src.errno_;

    return *this;
}

}