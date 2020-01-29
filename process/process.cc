#include "process.h"

int 
Process::create_process(void *child_arg, void *parent_arg)
{
    if ((pid_ = ::fork()) == -1) {
        return -1;
    } else if (pid_ == 0) {
        this->child_func(child_arg);
    } else {
        this->parent_func(parent_arg);
    }

    return 0;
}

int 
Process::wait_child_process(void)
{
    ::wait(NULL);
}