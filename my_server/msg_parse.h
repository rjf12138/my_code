#ifndef __HANDLE_MESSAGE_H__
#define __HANDLE_MESSAGE_H__
/*
*   等待从event_handle上来的数据，脱去消息的外壳
*   然后将上传到Task上, 文本消息会转为json格式，二进制只是解包网上
*   传，需要上层自行处理消息
*/
#include "basic_head.h"
#include "net_packet.h"
#include "thread.h"

class MessageHandler : public Thread, public MSGObject{
public:
    MessageHandler(OBJ_HANDLE task_handler);
    virtual ~MessageHandler(void);

    virtual int run_handler(void) override;
    virtual int exit_handler(void) override;
    virtual int on_msg(InnerMsg msg) override;

private:
    Buffer* get_buffer(int fd);

private:
    bool exit_ = false;
    OBJ_HANDLE task_handler_;
    map<int, shared_ptr<NetPacket>> io_buffer_;
};


#endif