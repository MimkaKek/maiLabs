#ifndef TARRAY_H
#define TARRAY_H

#include "TArrayItem.h"
#include "TIterator.h"

template<class T> 
class TArray {
public:
    TArray();
    
    std::ostream&                       Print(int step, std::ostream& os) const;
    std::shared_ptr<T>                  Pop(int i);
    
    bool                                Push(std::shared_ptr<T> &&figure);
    bool                                Empty();
    
    std::shared_ptr<TArrayItem<TQueue<T>>>*     GetArray();
    //TIterator<TArrayItem<TQueue<T>>,TQueue<T>>          begin();
    //TIterator<TArrayItem<TQueue<T>>,TQueue<T>>          end();
    
    template <class A> 
    friend std::ostream&                operator<<(std::ostream& os, TArray<A>& array);
    
    virtual ~TArray();
private:
    unsigned int                                current;
    unsigned int                                amount;
    std::shared_ptr<TArrayItem<TQueue<T>>>*     array;
};

#endif  /* TARRAY_H */
