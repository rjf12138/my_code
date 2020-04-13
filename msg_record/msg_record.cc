#include "msg_record.h"

namespace my_util {

MsgRecord::MsgRecord(void) 
{
}

MsgRecord::~MsgRecord(void)
{
}

void
MsgRecord::get_msg_info(int occur_line, string occur_file, const char *format, ...)
{
   
    uint32_t msg_len = strlen(format)+1;
    char *msg_buff = new char[msg_len];
    memset(msg_buff, 0, msg_len);

    va_list arg_ptr;
    va_start(arg_ptr,format);
    vsprintf(msg_buff, format, arg_ptr);
    va_end(arg_ptr);

    char strtime[65] = {0};
    time_t curr_time = time(NULL);
    strftime(strtime, sizeof(strtime), "%Y-%m-%d %H:%M:%S", localtime(&curr_time));

    msg_info_.when = strtime;
    msg_info_.msg_info = msg_buff;
    msg_info_.which_line = occur_line;
    msg_info_.which_file = basename(occur_file.c_str());
    delete msg_buff;

    return ;
}

void 
MsgRecord::get_msg_info(int occur_line, string occur_file, const string &msg)
{
    char strtime[65] = {0};
    time_t curr_time = time(NULL);
    strftime(strtime, sizeof(strtime), "%Y-%m-%d %H:%M:%S", localtime(&curr_time));
    
    msg_info_.when = strtime;
    msg_info_.msg_info = msg;
    msg_info_.which_line = occur_line;
    msg_info_.which_file = basename(occur_file.c_str());
}

void
MsgRecord::print_msg()
{
    std::cout << msg_info_.which_line << std::endl;
    std::cout << msg_info_.msg_info << std::endl;
}

}