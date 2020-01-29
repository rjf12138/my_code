#include "file.h"

FileOperate::FileOperate(void) :file_open_flag_(false)
{ }

FileOperate::~FileOperate(void)
{
    if (file_open_flag_ == true) {
        close(fd_);
    }
}

int 
FileOperate::open(const char *file_path, bool open_on_exit = false)
{
    fd_ = open(file_path);
}

int FileOperate::set_fd(int fd, bool open_on_exit = false);
int FileOperate::fileinfo(struct stat &file_info);