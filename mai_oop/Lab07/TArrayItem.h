#ifndef TARRAYITEM_H
#define TARRAYITEM_H
#include "TAllocationBlock.h"
#include <memory>

template<class T> class TArrayItem {
public:
    TArrayItem(const std::shared_ptr<T>& figure);
    
    std::shared_ptr<T>              GetValue() const;
    void                            Print() const;
    std::ostream&                   Print(std::ostream& os) const;
    
    void*                           operator new (size_t size);
    void                            operator delete(void *p);
    
    template<class A> 
    friend std::ostream&            operator << (std::ostream& os, const TArrayItem<A>& obj);
    
    template<class A>
    friend bool                     operator == (TArrayItem<A>& left, TArrayItem<A>& right);
    
    template<class A>
    friend bool                     operator != (TArrayItem<A>& left, TArrayItem<A>& right);
    
    template<class A>
    friend bool                     operator >  (TArrayItem<A>& left, TArrayItem<A>& right);
    
    template<class A>
    friend bool                     operator <  (TArrayItem<A>& left, TArrayItem<A>& right);
    
    template<class A>
    friend bool                     operator >= (TArrayItem<A>& left, TArrayItem<A>& right);
    
    template<class A>
    friend bool                     operator <= (TArrayItem<A>& left, TArrayItem<A>& right);
    
    virtual ~TArrayItem();
private:
    std::shared_ptr<T>              item;
    std::shared_ptr<TArrayItem<T>>  next;
    static TAllocationBlock         arrayitem_allocator;
};

#endif  /* TARRAYITEM_H */
