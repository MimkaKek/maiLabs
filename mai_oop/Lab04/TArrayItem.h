#ifndef TARRAYITEM_H
#define TARRAYITEM_H
#include <memory>

template<class P>
class TArray;

template<class T>
class TArrayItem {
public:

    friend TArray<T>; //TODO

    TArrayItem();
    TArrayItem(const std::shared_ptr<T>& figure);
    
    TArrayItem<T>*                      SetNext(TArrayItem<T>* next);
    TArrayItem<T>*                      GetNext() const;

    std::shared_ptr<T>                  SetItem(std::shared_ptr<T> &item);
    std::shared_ptr<T>                  GetItem() const;


    void* operator                      new (size_t size);
    void  operator                      delete (void *p);

    template<class A>
    friend std::ostream& operator       << (std::ostream& os, const TArrayItem<A>& obj);
    
    virtual ~TArrayItem();
private:

    std::shared_ptr<T>                  item;
    TArrayItem<T>*                      next;
};

#endif  /* TARRAYITEM_H */
