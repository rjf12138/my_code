#include "thread.h"

Thread::Thread(void) { }
Thread::~Thread(void) { }

void* 
Thread::create_func(void* arg)
{
    if (arg == NULL) {
        return NULL;
    }

    Thread *self = (Thread*)arg;
    self->run_handler();
}

int 
Thread::init(void)
{
    this->start_handler();
    return pthread_create(&thread_id_, NULL, create_func, (void*)this);
}

int Thread::run_handler(void) { }
int Thread::stop_handler(void) { }
int Thread::start_handler(void) { }

int Thread::wait_thread(void)
{
    return pthread_join(thread_id_, NULL);
}
