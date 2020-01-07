#include "queue.h"

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
    if(current_size == max_queue_size){
        queue_mutex_.unlock();
        return -1;
    }

    data_queue_.push(data);
    current_size++;
    queue_mutex_.unlock();
    return 0;
}

template<class T>
int Queue<T>::pop(T &data)
{
    queue_mutex_.lock();
    if(current_size == 0){
        queue_mutex_.unlock();
        return -1;
    }
    data = data_queue_.front();
    data_queue_.pop();
    current_size--;
    queue_mutex_.unlock();
    return 0;
}