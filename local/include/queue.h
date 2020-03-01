#ifndef __QUEUQ_H__
#define __QUEUQ_H__

#include "basic_head.h"
#include "mutex.h"

namespace my_util
{

template<class T>
class Queue{
public:
    Queue(void);
    ~Queue();

    int push(T &data);
    int pop(T &data);
    int size(void) {return current_size_;}
    bool empty(void) {return current_size_ == 0 ? true : false;}
private:
    ::queue<T> data_queue_;
    Mutex queue_mutex_;
    int max_queue_size_;
    int current_size_;
};

template<class T>
Queue<T>::Queue(void)
{
}

template<class T>
Queue<T>::~Queue()
{
 
}

template<class T>
int Queue<T>::push(T &data)
{
    queue_mutex_.lock();
    if(current_size_ == max_queue_size_){
        queue_mutex_.unlock();
        return -1;
    }

    data_queue_.push(data);
    current_size_++;
    queue_mutex_.unlock();
    return 0;
}

template<class T>
int Queue<T>::pop(T &data)
{
    queue_mutex_.lock();
    if(current_size_ == 0){
        queue_mutex_.unlock();
        return -1;
    }
    data = data_queue_.front();
    data_queue_.pop();
    current_size_--;
    queue_mutex_.unlock();
    return 0;
}

}

#endif
