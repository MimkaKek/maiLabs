#ifndef TQUEUEITEM_H
#define TQUEUEITEM_H

#include <memory>

template<class T> 
class TQueueItem {
public:
    TQueueItem(T* item);
    
    std::shared_ptr<TQueueItem<T>>  SetNext(std::shared_ptr<TQueueItem> &next);
    std::shared_ptr<TQueueItem<T>>  GetNext();
    T*                              GetValue() const;
    
    virtual ~TQueueItem();
private:
    T*                              item;
    std::shared_ptr<TQueueItem<T>>  next;
};

#endif  /* TQUEUEITEM_H */
