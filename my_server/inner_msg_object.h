#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "basic_head.h"
#include "queue.h"
#include "message.h"

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