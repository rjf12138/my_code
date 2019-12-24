#ifndef __QUEUQ_H__
#define __QUEUQ_H__

#include "basic_head.h"
#include "mutex.h"

template<class T>
class Queue{
public:
    Queue(void);
    ~Queue();

    int push(T &data);
    int pop(T &data);
    int size(void) {return current_size;}
    bool empty(void) {return current_size_ == 0 ? true : false;}
private:
    queue<T> data_queue_;
    Mutex queue_mutex_;
    int max_queue_size_;
    int current_size_;
};

#endif
