#ifndef __REACTOR_H__
#define __REACTOR_H__

#include "basic_head.h"
#include "event_handler.h"
#include "thread.h"

#define MAX_EVENT_NUM 512

class Reactor : public Thread ,public MSGObject{
public:
    Reactor(OBJ_HANDLE msg_handler);
    virtual ~Reactor(void);

    int init(void);
    int add_fd(int fd, bool is_listen_fd = false);
    int del_fd(int fd);
    int stop(void);

    virtual int run_handler() override;
    virtual int exit_handler() override;
    virtual int handler_event(int fd);

private:
    int setnonblocking(int fd);

private:
    int epoll_fd_ = -1;
    int listen_fd_ = -1;
    OBJ_HANDLE msg_handler_;
    bool exit_ = false;
    struct epoll_event event_[MAX_EVENT_NUM];
};

#endif