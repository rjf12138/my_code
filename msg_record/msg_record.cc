#include "msg_record.h"

namespace my_util {
//////////////////////////////////////////////
// 默认标准输出函数
int output_to_stdout(const string &msg)
{
    std::cout << msg;
}

// 默认标准出错函数
int output_to_stderr(const string &msg)
{
    std::cerr << msg;
}
//////////////////////////////////////////////

MsgRecord::MsgRecord(void)
: output_msg_to_stream_(output_to_stderr)
{

}

MsgRecord::~MsgRecord(void)
{
}

void
MsgRecord::print_msg(InfoLevel level, int line, string file_name, const char *format, ...)
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

    MsgContent tmp_msg;
    tmp_msg.info_level = level;
    tmp_msg.when = strtime;
    tmp_msg.msg_info = msg_buff;
    tmp_msg.which_line = line;
    tmp_msg.which_file = basename(file_name.c_str());
    msg_info_.push_back(tmp_msg);
    delete msg_buff;
    // 组装缓存中的所有消息
    string print_msg = this->assemble_msg();
    // 向流中输出消息
    this->output_msg_to_stream_(print_msg);

    return ;
}

string 
MsgRecord::get_msg_attr(InfoLevel level, int line, string file_name, const char *format, ...)
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

    MsgContent tmp_msg;
    tmp_msg.info_level = level;
    tmp_msg.when = strtime;
    tmp_msg.msg_info = msg_buff;
    tmp_msg.which_line = line;
    tmp_msg.which_file = basename(file_name.c_str());
    msg_info_.push_back(tmp_msg);
    delete msg_buff;
    // 组装缓存中的所有消息
    return this->assemble_msg();
}

string
MsgRecord::assemble_msg(void)
{
    ostringstream ostr;
    for (int i = 0; i < msg_info_.size(); ++i) {
        ostr << "[" << msg_info_[i].when << "]";
        ostr << "[" << msg_info_[i].which_line << "]";
        ostr << "[" << msg_info_[i].which_file << "] ";
        ostr << "[" << level_convert(msg_info_[i].info_level) << "]";
        ostr << msg_info_[i].msg_info << std::endl;
    }
    
    return ostr.str();
}

string 
MsgRecord::level_convert(enum InfoLevel level)
{
    switch(level)
    {
        case LOG_LEVEL_TRACE:
            return "TRACE";
        case LOG_LEVEL_DEBUG:
            return "DEBUG";
        case LOG_LEVEL_INFO:
            return "INFO";
        case LOG_LEVEL_WARN:
            return "WARN";
        case LOG_LEVEL_ERROR:
            return "ERROR";
        case LOG_LEVEL_FATAL:
            return "FATAL";
    }

    return "UNKNOWN_LOG_LEVEL";
}

}