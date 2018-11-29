#ifndef TQUEUE_H
#define TQUEUE_H

#include "TIterator.h"
#include "TQueueItem.h"
#include <memory>

template <typename T> 
class TQueue {
public:
    TQueue();
    
    void push(std::shared_ptr<T> &&item);
    bool empty();
    size_t GetSize();
    
    
    TIterator<TQueueItem<T>,T> begin();
    TIterator<TQueueItem<T>,T> end();
    
    std::shared_ptr<T> pop();
    template <typename A> 
    friend std::ostream& operator<<(std::ostream& os,const TQueue<A>& queue);
    
    virtual ~TQueue();
private:
    size_t size;
    std::shared_ptr<TQueueItem<T>> tail;
    std::shared_ptr<TQueueItem<T>> head;
};

#endif  /* TQUEUE_H */
