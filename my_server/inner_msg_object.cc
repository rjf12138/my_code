#include "inner_msg_object.h"

OBJ_HANDLE MSGObject::next_object_id_ = -1;

MSGObject::MSGObject(void)
{
    if (next_object_id_ = -1) {
        next_object_id_ = 2000; // start obj_id
    }

    object_id_ = next_object_id_++;
    obj_map_[object_id_] = this;
}

MSGObject::~MSGObject(void)
{
    obj_map_.erase(object_id_);
}

int MSGObject::on_msg(InnerMsg msg)
{
    return 0;
}

int MSGObject::trans_msg(OBJ_HANDLE hander, const InnerMsg &msg)
{
    auto iter = obj_map_.find(hander);
    if (iter == obj_map_.end()) {
        return -1;
    }

    iter->second->on_msg(msg);

    return 0;
}