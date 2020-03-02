#ifndef  __ERROR_H__
#define  __ERROR_H__

#include "basic_head.h"
#include "queue.h"

namespace my_util {

#define ERR_MSG_BUFF 512

enum ERROR_TYPE {
    ERROR_DEF_ERR = 1,
    ERROR_SYS_ERR = 2,
};

enum ERROR_OUTPUT_TO {
    ERROR_TO_STDERR = 3,
    ERROR_TO_LOG = 4,
};

enum ERROR_OUTPUT_TYPE {
    ERROR_OUTPUT_IMMEDIATELY = 5,
    ERROR_OUTPUT_OVERFLOW = 6,
};

typedef int32_t ERR_OBJECT_ID;

class ErrorHandler {
public:
    ErrorHandler(void);
    virtual ~ErrorHandler(void);

    // err_type 分为系统的错误和我们自己定义的错误
    static int input_err_info(int occur_line, string occur_file, const char *format, ...);
    // output_to:   选择输出位置可以是屏幕或是以后的日志系统
    // output_type: 可以选择有错误时立即输出，或是等到朝过overflow_size设定值在全部输出
    int set_err_output(int output_to = ERROR_TO_STDERR, 
                int output_type = ERROR_OUTPUT_IMMEDIATELY, int overflow_size = 0);
    
    static int input_static_err_info(ErrorHandler err_handle, int occur_line, string occur_file, const char *format, ...);

protected:
    static map<ERR_OBJECT_ID, Queue<string>> obj_map_;
    ERR_OBJECT_ID err_id_;

private:
    void print_err_info(string err_msg);

private:
    int output_to_;
    int output_type_;
    int overflow_size_;

    static ERR_OBJECT_ID next_err_id_;
    static ERR_OBJECT_ID static_func_err_id_;
};

#define err_msg(...)        ErrorHandler::input_err_info(__LINE__, __FILE__, __VA_ARGS__)
#define err_msg_static(...) ErrorHandler::input_err_info(__LINE__, __FILE__, __VA_ARGS__)

}

#endif