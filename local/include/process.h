#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "basic_head.h"

class Process {
public:
    Process(void) {}
    ~Process(void) {}

    int create_process(void *child_arg, void *parent_arg);
    int wait_child_process(void);
    virtual int child_func(void *arg) {}
    virtual int parent_func(void *arg) {}

private:
    int pid_;
};

#endif