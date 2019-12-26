#ifndef __REACTOR_H__
#define __REACTOR_H__

#include "basic_head.h"
#include "thread.h"

#define MAX_MSG_BUF 8192
#define MAX_EVENT_NUM 512

class Reactor : public Thread ,public MSGObject {
public:
    Reactor(OBJ_HANDLE manager_hander);
    virtual ~Reactor(void);

    int start(void);
    int stop(void);
    int init(void);
    int add_fd(int fd, bool is_listen_fd = false);
    int del_fd(int fd);
    int stop(void);
    int handler_event(int fd);

    virtual int run_handler() override;
    virtual int exit_handler() override;
    virtual int on_msg(InnerMsg msg) override;

private:
    int setnonblocking(int fd);

private:
    int epoll_fd_ = -1;
    int listen_fd_ = -1;
    bool exit_ = false;
    struct epoll_event event_[MAX_EVENT_NUM];
    OBJ_HANDLE manager_hander_;
};

#endif