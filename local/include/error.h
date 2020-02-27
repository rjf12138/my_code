#ifdef  __ERROR_H__
#define __ERROR_H__

#include "basic_head.h"
#include "queue.h"

namespace my_util {

enum ERROR_TYPE {
    ERROR_DEF_ERR = 1,
    ERROR_SYS_ERR = 2,
};

enum ERROR_OUTPUT_TO {
    ERROR_TO_STDERR = 3,
    ERROR_TO_LOG = 4,
};

enum ERROR_OUTPUT_TYPE {
    ERROR_OUTPUT_IMMEDIATELY = 5;
    ERROR_OUTPUT_OVERFLOW = 6;
};

struct ErrInfo {
    int err_type_;
    int err_num_;
    int occur_line_;
    time_t occur_time_;
    string occur_file_;
    string err_msg_;
};

typedef int16_t ERR_OBJECT_ID;

class Error {
public:
    Error(void);
    virtual ~Error(void);

    static int input_err_info(int err_num, string err_msg, int err_type = ERROR_DEF_ERR \
                        int occur_line = __LINE__, string occur_file = __FILE__);
    // output_to:   选择输出位置可以是屏幕或是以后的日志系统
    // output_type: 可以选择有错误时立即输出，或是等到朝过overflow_size设定值在全部输出
    static int set_err_output(int output_to = ERROR_TO_STDERR, \
                int output_type = ERROR_OUTPUT_IMMEDIATELY, int overflow_size = 0);

protected:
    static map<ERR_OBJECT_ID, Queue<ErrInfo>> obj_map_;
    ERR_OBJECT_ID err_id_;
private:
    void print_err_info(ErrInfo &err_info);
private:
    static ERR_OBJECT_ID next_err_id_;
};

};

#endif