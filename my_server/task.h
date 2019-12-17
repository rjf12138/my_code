// 这个类要改为根据 msg 处理具体事务的类， Server 则是用来服务的启动和消息的传递和管理

#ifndef __TASK_H__
#define __TASK_H__

#include "basic_head.h"

class Task : public Thread, public MSGObject{
public:
    Task(string ip, short port, int timeout = 100);
    virtual ~Task(void);

    virtual void process_task(void);
    virtual void run(void);
    virtual void stop(void);

private:
private:
    bool exit_;
};

#endif