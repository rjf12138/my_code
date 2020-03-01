#ifndef __FILE_H__
#define __FILE_H__

#include "basic_head.h"
#include "byte_buffer.h"
#include "error_handle.h"

namespace my_util {
#define DEFAULT_OPEN_FLAG   O_RDWR
#define DEFAULT_FILE_RIGHT  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

class FileOperate : public ErrorHandler {
public:
    FileOperate(void);
    ~FileOperate(void);

    // 根据文件路径打开文件
    int open(const string file_path, int flag = DEFAULT_OPEN_FLAG, int file_right = DEFAULT_FILE_RIGHT);
    // 根据文件描述符打开文件
    int set_fd(int fd, bool open_on_exit = true);

    int fileinfo(struct stat &file_info);
    int close_file(void);
    int check_fd(int fd);
    void print_errno(void);

    // 设置文件偏移
    off_t seek(off_t offset, int whence);
    // 返回当前位置
    off_t curr_pos(void);

    // 从当前位置读取任意数据
    ssize_t read(ByteBuffer &buff, size_t buf_size);
    // 从某一位置读取数据
    ssize_t read_from_pos(ByteBuffer &buff, size_t buf_size, off_t pos, int whence);

    // 写数据
    ssize_t write(ByteBuffer buff, size_t buf_size);
    // 从某一位置写数据
    ssize_t write_to_pos(ByteBuffer buff, size_t buf_size ,off_t pos, int whence);

    // 从标准输入读
    static ssize_t read_from_stdin(ByteBuffer &buff);
    // 写到标准输出
    static ssize_t write_to_stdout(ByteBuffer buff, size_t buf_size);
    // 写到标准出错
    static ssize_t write_to_stderr(ByteBuffer buff, size_t buf_size);

private:
    int errno_;
    int fd_;
    bool open_on_exit_;
    bool file_open_flag_;
    unsigned max_lines_;
    struct stat file_info_;
};

}
#endif