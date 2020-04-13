#ifndef  __ERROR_H__
#define  __ERROR_H__

#include "basic_head.h"

namespace my_util {

#define ERR_MSG_BUFF 512

struct MsgContent {
    string when;
    string which_file;
    string which_line;
    string msg_info;
};

class MsgRecord {
public:
    MsgRecord(void);
    virtual ~MsgRecord(void);

    static void get_msg_info(int line, string file_name, const char *format, ...);
    static void get_msg_info(int line, string file_name, const string &msg);
    static MsgContent get_all_msg(void) {return msg_info_;}

    static void print_msg(void);
private:
    static MsgContent msg_info_;
};

#define get_msg(...)         MsgRecord::get_msg_info(__LINE__, __FILE__,__VA_ARGS__)
#define print_msg()          MsgRecord::print_msg()

}

#endif