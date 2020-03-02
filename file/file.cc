#include "file.h"

namespace my_util {

FileOperate::FileOperate(void) :file_open_flag_(false)
{ }

FileOperate::~FileOperate(void)
{
    this->close_file();
}

int 
FileOperate::open(const string file_path, int flag, int file_right)
{
    if (file_path.empty()) {
        fprintf(stderr, "file_path is empty");
        return -1;
    }

    int fd = -1;
    // 检查文件是否存在
    int ret = ::access(file_path.c_str(), F_OK);
    if (ret == -1) {
        fd = ::open(file_path.c_str(), flag, file_right);
    } else {
        fd = ::open(file_path.c_str(), flag);
    }

    if (fd == -1) {
        fprintf(stderr, "open: %s\n", strerror(errno));
        return -1;
    }
    // 获取文件信息
    struct stat info;
    ret = ::stat(file_path.c_str(), &info);
    if (ret == -1)
    {
        fprintf(stderr, "stat: %s\n", strerror(errno));
        return -1;
    }

    // 如果之前有打开文件，先关闭之前的文件
    this->close_file();
    fd_ = fd;
    file_info_ = info;
    file_open_flag_ = true;
    open_on_exit_ = true;

    return 0;
}

int 
FileOperate::set_fd(int fd, bool open_on_exit)
{
    if (this->check_fd(fd) == -1) {
        return -1;
    }
    this->close_file();

    fd_ = fd;
    int ret = ::fstat(fd_, &file_info_);
    if (ret == -1) {
        file_open_flag_ = false;
        fprintf(stderr, "fstat: %s", strerror(errno));
        return -1;
    }
    open_on_exit_ = open_on_exit;

    return 0;
}

int 
FileOperate::fileinfo(struct stat &file_info)
{
    if (file_open_flag_ == true) {
        file_info = file_info_;
    } else {
        fprintf(stderr, "haven't open any file.\n");
        return -1;
    }

    return 0;
}

int 
FileOperate::close_file(void)
{
    if (file_open_flag_ == true && open_on_exit_) {
        file_open_flag_ = false;
        close(fd_);
    }
}

int 
FileOperate::check_fd(int fd)
{
    struct stat info;
    int ret = fstat(fd, &info);
    if (ret == -1) {
        fprintf(stderr, "fstat: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}

off_t 
FileOperate::seek(off_t offset, int whence)
{
    off_t pos = lseek(fd_, offset, whence);
    if (pos == -1) {
        fprintf(stderr, "lseek: %s\n", strerror(errno));
        return -1;
    }

    return pos;
}

off_t 
FileOperate::curr_pos(void)
{
    return this->seek(0, SEEK_CUR);
}

ssize_t 
FileOperate::read(ByteBuffer &buff, size_t buf_size)
{
    if (!file_open_flag_) {
        fprintf(stderr, "FileOperate::read: haven't open any file!\n");
        return -1;
    }

    const int tmp_buf_size = buf_size;
    int8_t buf[tmp_buf_size] = {0};
    ssize_t ret = ::read(fd_, buf, tmp_buf_size);
    if (ret == -1) {
        fprintf(stderr, "read: %s\n", strerror(errno));
        return ret;
    }

    buff.write_bytes(buf, ret);
    return ret;
}

ssize_t 
FileOperate::read_from_pos(ByteBuffer &buff, size_t buf_size, off_t pos, int whence)
{
    if (!file_open_flag_) {
        fprintf(stderr, "read_from_pos: haven't open any file!\n");
        return -1;
    }

    if (this->seek(pos, whence) == -1) {
        fprintf(stderr, "seek: %s!\n", strerror(errno));
        return -1;
    }

    int ret_size = this->read(buff, buf_size);
    return ret_size;
}


ssize_t 
FileOperate::write(ByteBuffer buff, size_t buf_size)
{
    if (!file_open_flag_) {
        fprintf(stderr, "write: haven't open any file!\n");
        return -1;
    }

    const int tmp_buf_size = buff.data_size();
    int8_t buf[tmp_buf_size] = {0};
    buff.read_bytes(buf, tmp_buf_size);
    ssize_t ret = ::write(fd_, buf, tmp_buf_size);
    if (ret == -1) {
        fprintf(stderr, "write: %s!\n", strerror(errno));
        return -1;
    }

    return ret;
}

ssize_t 
FileOperate::write_to_pos(ByteBuffer buff, size_t buf_size ,off_t pos, int whence)
{
    if (!file_open_flag_) {
        fprintf(stderr, "write_to_pos: haven't open any file!\n");
        return -1;
    }

    if (this->seek(pos, whence) == -1) {
        fprintf(stderr, "seek: %s!\n", strerror(errno));
        return -1;
    }

    int ret_size = this->write(buff, buf_size);
    return ret_size;
}

ssize_t 
FileOperate::read_from_stdin(ByteBuffer &buff)
{
    const int buf_size = 256;
    int8_t buf[buf_size] = {0};
    ssize_t ret = ::read(STDIN_FILENO, buf, buf_size);
    if (ret == -1) {
        fprintf(stderr, "read: %s!\n", strerror(errno));
        return ret;
    }

    buff.write_bytes(buf, ret);
    return ret;
}

ssize_t 
FileOperate::write_to_stdout(ByteBuffer buff, size_t buf_size)
{
    const int tmp_buf_size = buff.data_size();
    int8_t buf[tmp_buf_size] = {0};
    buff.read_bytes(buf, tmp_buf_size);
    ssize_t ret = ::write(STDOUT_FILENO, buf, tmp_buf_size);
    if (ret == -1) {
        fprintf(stderr, "write: %s!\n", strerror(errno));
        return -1;
    }
    
    return ret;
}

ssize_t 
FileOperate::write_to_stderr(ByteBuffer buff, size_t buf_size)
{
    const int tmp_buf_size = buff.data_size();
    int8_t buf[tmp_buf_size] = {0};
    buff.read_bytes(buf, tmp_buf_size);
    ssize_t ret = ::write(STDERR_FILENO, buf, tmp_buf_size);
    if (ret == -1) {
        fprintf(stderr, "write: %s!\n", strerror(errno));
        return -1;
    }

    return ret;
}

}