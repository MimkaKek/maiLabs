#ifndef TARRAYITEM_H
#define TARRAYITEM_H
#include "TAllocationBlock.h"
#include "TQueue.h"
#include <memory>

template<class T> class TArrayItem {
public:
    TArrayItem();
    
    std::shared_ptr<TArrayItem<T>>  SetNext(std::shared_ptr<TArrayItem<T>>& next);
    std::shared_ptr<TArrayItem<T>>  GetNext() const;
    std::shared_ptr<T>      GetValue() const;
    void                            Print() const;
    std::ostream&                   Print(std::ostream& os) const;
    
    void*                           operator new (size_t size);
    void                            operator delete(void *p);
    
    template<class A> 
    friend std::ostream&            operator<<(std::ostream& os, const TArrayItem<A>& obj);
    
    virtual ~TArrayItem();
private:
    std::shared_ptr<T>              item;
    std::shared_ptr<TArrayItem<T>>  next;
    static TAllocationBlock         arrayitem_allocator;
};

#endif  /* TARRAYITEM_H */
