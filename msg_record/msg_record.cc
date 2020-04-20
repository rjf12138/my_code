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

    MsgContent tmp_msg;
    tmp_msg.when = strtime;
    tmp_msg.msg_info = msg_buff;
    tmp_msg.which_line = occur_line;
    tmp_msg.which_file = basename(occur_file.c_str());
    msg_info_.push_back(tmp_msg);

    delete msg_buff;

    return ;
}

void 
MsgRecord::get_msg_info(int occur_line, string occur_file, const string &msg)
{
    char strtime[65] = {0};
    time_t curr_time = time(NULL);
    strftime(strtime, sizeof(strtime), "%Y-%m-%d %H:%M:%S", localtime(&curr_time));
    
    MsgContent tmp_msg;
    tmp_msg.when = strtime;
    tmp_msg.msg_info = msg;
    tmp_msg.which_line = occur_line;
    tmp_msg.which_file = basename(occur_file.c_str());
    msg_info_.push_back(tmp_msg);

    return ;
}

string
MsgRecord:: assembe_msg(void)
{
    ostringstream ostr;
    for (int i = 0; i < msg_info_.size(); ++i) {
        ostr << "[" << msg_info_[i].when << "]";
        ostr << "[" << msg_info_[i].which_line << "]";
        ostr << "[" << msg_info_[i].which_file << "] ";
        ostr << msg_info_[i].msg_info << std::endl;
    }

    return ostr.str();
}

}