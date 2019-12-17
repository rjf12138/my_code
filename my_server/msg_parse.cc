#include "msg_parse.h"

MessageHandler::MessageHandler(OBJ_HANDLE task_handler): 
    task_handler_(task_handler) 
{ }

MessageHandler::~MessageHandler(void) 
{
    for (auto iter = io_buffer_.begin(); iter != io_buffer_.end(); ++iter) {
        Buffer *del_buffer = iter->second;
        delete del_buffer;
        del_buffer = nullptr;
    }
}

Buffer* 
MessageHandler::get_buffer(int fd)
{
    auto iter = io_buffer_.find(fd);
    if (iter == io_buffer_.end()) {
        Buffer *buffer = new Buffer(MAX_MSG_LENGTH * 4);
        io_buffer_[fd] = buffer;

        return buffer; 
    }

    return iter->second;
}

int MessageHandler::run_handler(void)
{
    while (!exit_) {
        
    }
}

int MessageHandler::exit_handler(void)
{
    exit_ = true;
}

int 
MessageHandler::on_msg(InnerMsg msg)
{
    switch (msg.msg_id_)
    {
    case MSG_DATA_BUFFER:
    {
        int fd = 
    } 
    break;
    default:
        break;
    }
}