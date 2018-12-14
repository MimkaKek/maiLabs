#ifndef TQUEUEITEM_H
#define TQUEUEITEM_H

#include <memory>

template<class T> 
class TQueueItem {
public:
    TQueueItem(const std::shared_ptr<T>& figure);
    
    template<class A> 
    friend std::ostream& operator<<(std::ostream& os, const TQueueItem<A>& obj);
    
    std::shared_ptr<TQueueItem<T>>  SetNext(std::shared_ptr<TQueueItem> next);
    std::shared_ptr<TQueueItem<T>>  GetNext();
    std::shared_ptr<T>              GetValue() const;
    
    virtual ~TQueueItem();
private:
    std::shared_ptr<T>              item;
    std::shared_ptr<TQueueItem<T>>  next;
};

#endif  /* TQUEUEITEM_H */
