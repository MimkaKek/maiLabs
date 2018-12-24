#ifndef TQUEUE_H
#define TQUEUE_H

#include "TQueueItem.h"
#include <memory>

template <class T> 
class TQueue {
public:
    TQueue();
    
    void push(T* item);
    
    bool empty();
    
    T* pop();
    
    virtual ~TQueue();
private:
    size_t size;
    std::shared_ptr<TQueueItem<T>> tail;
    std::shared_ptr<TQueueItem<T>> head;
};

#endif  /* TQUEUE_H */
