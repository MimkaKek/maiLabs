#ifndef TARRAY_H
#define TARRAY_H

#include "TIterator.h"
#include "TArrayItem.h"
#include <memory>

template<class T> class TArray {
public:
    TArray();
  
    void                            Push(std::shared_ptr<T> figure);
    std::shared_ptr<T>              Pop(int i);
  
  
    void                            Print(int step) const;
    bool                            Empty();
  
    TIterator<TArray<T>,TArrayItem<T>>          begin();
    TIterator<TArray<T>,TArrayItem<T>>          end();
  
    template <class A>
    friend std::ostream& operator   << (std::ostream& os, TArray<A>& array);

    virtual ~TArray();
private:

    void                            SetNewSize(unsigned int nSize);
    
    unsigned int                    Max;
    unsigned int                    Current;

    std::shared_ptr<TArrayItem<T>>  End;
    std::shared_ptr<TArrayItem<T>>* Array;
};

#endif  /* TARRAY_H */
