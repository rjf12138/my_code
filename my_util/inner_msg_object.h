#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "basic_head.h"
#include "queue.h"

////////////////////// Inner Msg Struct //////////////////////////

#include "inner_msg_object.h"

struct InnerMsg {
    int msg_id_;
    OBJ_HANDLE handler_;
    int param1_;
    int param2_;
    string str_;
    shared_ptr<void> object_;

    InnerMsg(int msg_id = 0, OBJ_HANDLE handler = 0, int param1 = 0, int param2 = 0, string str = "", void* object = nullptr):
        msg_id_(msg_id), handler_(handler), param1_(param1), param2_(param2),str_(str), object_(object)
    {}
};



typedef uint8_t OBJ_HANDLE;

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