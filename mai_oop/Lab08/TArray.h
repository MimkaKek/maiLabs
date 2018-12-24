#ifndef TARRAY_H
#define TARRAY_H

#include "TArrayItem.h"
#include "TIterator.h"
#include <memory>
#include <future>
#include <mutex>
#include <iostream>

template<class T> 
class TArray {
public:
    TArray();
    
    std::ostream&                       Print(int step, std::ostream& os) const;
    std::shared_ptr<T>                  Pop(int i);
    
    bool                                Push(std::shared_ptr<T> figure);
    bool                                Empty();
    
    size_t                              Size();
    
    std::shared_ptr<TArrayItem<T>>*     GetArray();
    void                                Reverse();
    
    TIterator<TArrayItem<T>,T>          begin();
    TIterator<TArrayItem<T>,T>          end();
    
    template <class A> 
    friend std::ostream&                operator << (std::ostream& os, TArray<A>& array);
    
    virtual ~TArray();
private:
    unsigned int                        current;
    unsigned int                        size;
    unsigned int                        amount;
    std::shared_ptr<TArrayItem<T>>*     array;
};

#endif  /* TARRAY_H */
