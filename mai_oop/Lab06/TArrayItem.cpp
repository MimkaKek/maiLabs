#include "TArrayItem.h"
#include <iostream>

template <class T> 
TArrayItem<T>::TArrayItem() {
    std::shared_ptr<T> tmp(new T);
    this->item = tmp;
    this->next = nullptr;
}

template <class T>
TAllocationBlock TArrayItem<T>::arrayitem_allocator(sizeof(TArrayItem<T>),16);

template <class T> 
std::shared_ptr<T> TArrayItem<T>::GetValue() const {
    return this->item;
}

template <class T> 
std::shared_ptr<TArrayItem<T>> TArrayItem<T>::GetNext() const {
    return this->next;
}

template <class T> 
std::shared_ptr<TArrayItem<T>> TArrayItem<T>::SetNext(std::shared_ptr<TArrayItem<T>>& next) {
    std::shared_ptr<TArrayItem<T>> old = this->next;
    this->next = next;
    return old;
}

template <class T> 
void TArrayItem<T>::Print() const {
    std::cout << *(this->item) << std::endl;
    return;
}

template <class T> 
std::ostream& TArrayItem<T>::Print(std::ostream& os) const {
    os << *(this->item) << std::endl;
    return os;
}

template <class T> 
TArrayItem<T>::~TArrayItem() {
}

template <class A> 
std::ostream& operator<<(std::ostream& os, const TArrayItem<A>& obj) {
    os << *obj.item << std::endl;
    return os;
}

template <class T> 
void* TArrayItem<T>::operator new (size_t size) {
    return arrayitem_allocator.allocate();
}


template <class T> 
void TArrayItem<T>::operator delete(void *p) {
    arrayitem_allocator.deallocate(p);
}


#include "Figure.h"
#include "TQueue.h"
template class TArrayItem<TQueue<Figure>>;
template std::ostream& operator<<(std::ostream& os, const TArrayItem<TQueue<Figure>>& obj);

