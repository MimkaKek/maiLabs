#ifndef TQUEUE_H
#define TQUEUE_H

#include "TIterator.h"
#include "TQueueItem.h"
#include <memory>
#include <iostream>

template <class T> 
class TQueue {
public:
    TQueue();
    
    void push(std::shared_ptr<T> item);
    bool empty();
    size_t GetSize();
    
    //TIterator<TQueueItem<T>,T> begin();
    //TIterator<TQueueItem<T>,T> end();
    
    std::shared_ptr<T> pop(int step);
    template <class A> 
    friend std::ostream& operator<<(std::ostream& os,const TQueue<A>& queue);
    
    virtual ~TQueue();
private:
    size_t size;
    std::shared_ptr<TQueueItem<T>> head;
};

#endif  /* TQUEUE_H */
