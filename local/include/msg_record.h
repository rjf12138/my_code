#ifndef  __MSG_RECORD_H__
#define  __MSG_RECORD_H__

#include "basic_head.h"
#include "file.h"

namespace my_util {

enum InfoLevel {
    LOG_LEVEL_TRACE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
};

struct MsgContent {
    InfoLevel info_level;
    int which_line;
    string when;
    string which_file;
    string msg_info;
};

/////////////////////////////////////////////////////
// 默认标准输出函数
int output_to_stdout(const string &msg);

// 默认标准出错函数
int output_to_stderr(const string &msg);

/////////////////////////////////////////////////////

class MsgRecord {
public:
    MsgRecord(void);
    virtual ~MsgRecord(void);

    virtual void print_msg(InfoLevel level, int line, string file_name, const char *format, ...);
    virtual string get_msg_attr(InfoLevel level, int line, string file_name, const char *format, ...);
    virtual string assemble_msg(void);
    string level_convert(enum InfoLevel level);

    void set_stream_func(int (*func)(const string &msg)) {
        output_msg_to_stream_ = func;
    }
private:
    // 消息缓存
    vector<MsgContent> msg_info_;
    // 输出函数变量 
    int (*output_msg_to_stream_)(const string &msg);
};

#define output_msg_trace(...)  this->print_msg(LOG_LEVEL_TRACE, __LINE__, __FILE__, __VA_ARGS__)
#define output_msg_debug(...)  this->print_msg(LOG_LEVEL_DEBUG, __LINE__, __FILE__, __VA_ARGS__)
#define output_msg_info(...)  this->print_msg(LOG_LEVEL_INFO, __LINE__, __FILE__, __VA_ARGS__)
#define output_msg_warning(...)  this->print_msg(LOG_LEVEL_WARN, __LINE__, __FILE__, __VA_ARGS__)
#define output_msg_error(...)  this->print_msg(LOG_LEVEL_ERROR, __LINE__, __FILE__, __VA_ARGS__)
#define output_msg_fatal(...)  this->print_msg(LOG_LEVEL_FATAL, __LINE__, __FILE__, __VA_ARGS__)

#define get_msg(...)  this->get_msg_attr(LOG_LEVEL_INFO, __LINE__, __FILE__, __VA_ARGS__)
}

#endif