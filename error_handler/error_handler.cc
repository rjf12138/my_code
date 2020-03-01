#include "error_handler.h"

namespace my_util {

map<ERR_OBJECT_ID, Queue<string>> ErrorHandler::obj_map_;
ERR_OBJECT_ID ErrorHandler::next_err_id_ = 100;
ERR_OBJECT_ID ErrorHandler::static_func_err_id_ = -1001;

ErrorHandler::ErrorHandler(void) 
{
    output_to_ = ERROR_TO_STDERR;
    output_type_ = ERROR_OUTPUT_IMMEDIATELY;
    overflow_size_ = overflow_size_;
    
    obj_map_[err_id_] = Queue<string>();
    next_err_id_++;

    if (obj_map_.find(static_func_err_id_) == obj_map_.end()) {
        obj_map_[static_func_err_id_] = Queue<string>();
    }
}

ErrorHandler::~ErrorHandler(void)
{
    string err;
    for (auto iter = obj_map_.begin(); iter != obj_map_.end(); ++iter) {
        while (!iter->second.empty()) {
            iter->second.pop(err);
            print_err_info(err);
        }
    }
}

void 
ErrorHandler::print_err_info(string err_msg)
{                       
    switch (output_to_) {
        case ERROR_TO_STDERR:
        {
            fprintf(stderr, "%s\n", err_msg.c_str());
        } break;
        case ERROR_TO_LOG:
        {

        } break;
        default:
        {
            fprintf(stderr, "print_err_info unknown option.");
        } break;
    }
}

int 
ErrorHandler::input_err_info(int occur_line, string occur_file, const char *format, ...)
{
    char log_msg_buffer[ERR_MSG_BUFF];
    memset(log_msg_buffer,'\0',sizeof(log_msg_buffer));

    char strtime[65] = {0};
    time_t curr_time = time(NULL);
    strftime(strtime, sizeof(strtime), "%Y-%m-%d %H:%M:%S", localtime(&curr_time));
    sprintf(log_msg_buffer, "[%s(%d)]: ", basename(occur_file.c_str()), occur_line);
    string log_msg = string(log_msg_buffer);
    memset(log_msg_buffer,'\0',sizeof(log_msg_buffer));

    va_list arg_ptr;
    va_start(arg_ptr,format);

    vsprintf(log_msg_buffer, format, arg_ptr);
    log_msg = log_msg + log_msg_buffer;

    va_end(arg_ptr);

    if (output_type_ == ERROR_OUTPUT_IMMEDIATELY) {
        this->print_err_info(log_msg);
    } else if (output_type_ == ERROR_OUTPUT_OVERFLOW){
        obj_map_[err_id_].push(log_msg);
        if (obj_map_[err_id_].size() >= overflow_size_) {
            while (! obj_map_[err_id_].empty()) {
                obj_map_[err_id_].pop(log_msg);
                this->print_err_info(log_msg);
            }
        }
    }

    return 0;
}

int 
ErrorHandler::input_static_err_info(ErrorHandler err_handle, int occur_line, string occur_file, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    err_handle.input_err_info(occur_line, occur_file, format, args);
    va_end(args);
}

int 
ErrorHandler::set_err_output(int output_to, int output_type, int overflow_size)
{
    output_to_ = output_to;
    output_type_ = output_type;
    overflow_size_ = overflow_size;
}
}