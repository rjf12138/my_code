#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "basic_head.h"
#include "queue.h"

////////////////////// Inner Msg Struct //////////////////////////

#include "inner_msg_object.h"

/// 解决参数类型不对的问题

struct InnerMsg {
    int msg_id_;
    OBJ_HANDLE handler_;
    int param1_;
    int param2_;
    shared_ptr<Buffer> buffer_;

    InnerMsg(int msg_id = 0, OBJ_HANDLE handler = 0, int param1 = 0, int param2 = 0):
        msg_id_(msg_id), handler_(handler), param1_(param1), param2_(param2)
    {}

    void set_buffer(shared_ptr<Buffer> buffer) {buffer_ = buffer;}
};



typedef uint16_t OBJ_HANDLE;

class MSGObject {
public:
    MSGObject(void);
    virtual ~MSGObject(void);

    OBJ_HANDLE get_handler(void) const { return object_id_; }
    virtual int on_msg(InnerMsg msg);

    static int trans_msg(OBJ_HANDLE hander, const InnerMsg &msg);

protected:
    static map<OBJ_HANDLE, MSGObject*> obj_map_;
    static Queue<InnerMsg> msg_queue_;

    OBJ_HANDLE object_id_;
private:
    static OBJ_HANDLE next_object_id_;
};

#endif