#include "TArrayItem.h"
#include <iostream>

template <class T> 
TArrayItem<T>::TArrayItem(const std::shared_ptr<T>& item) {
    this->item = item;
    this->next = nullptr;
}

template <class T>
TAllocationBlock TArrayItem<T>::arrayitem_allocator(sizeof(TArrayItem<T>),100);

template <class T> 
std::shared_ptr<T> TArrayItem<T>::GetValue() const {
    return this->item;
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

template <class A>
bool operator == (TArrayItem<A>& left, TArrayItem<A>& right){
        return *(left.GetValue()) == *(right.GetValue());
}

template <class A>
bool operator != (TArrayItem<A>& left, TArrayItem<A>& right){
        return *(left.GetValue()) != *(right.GetValue());
}

template <class A>
bool operator >  (TArrayItem<A>& left, TArrayItem<A>& right){
        return *(left.GetValue()) > (*right.GetValue());
}

template <class A>
bool operator <  (TArrayItem<A>& left, TArrayItem<A>& right){
        return *(left.GetValue()) < *(right.GetValue());
}

template <class A>
bool operator >= (TArrayItem<A>& left, TArrayItem<A>& right){
        return *(left.GetValue()) >= *(right.GetValue());
}

template <class A>
bool operator <= (TArrayItem<A>& left, TArrayItem<A>& right){
        return *(left.GetValue()) <= *(right.GetValue());
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
template class TArrayItem<Figure>;
template std::ostream&  operator << (std::ostream& os, const TArrayItem<Figure>& obj);
template bool           operator <  (TArrayItem<Figure>& left, TArrayItem<Figure>& right);
template bool           operator >  (TArrayItem<Figure>& left, TArrayItem<Figure>& right);
template bool           operator <= (TArrayItem<Figure>& left, TArrayItem<Figure>& right);
template bool           operator >= (TArrayItem<Figure>& left, TArrayItem<Figure>& right);
template bool           operator == (TArrayItem<Figure>& left, TArrayItem<Figure>& right);
template bool           operator != (TArrayItem<Figure>& left, TArrayItem<Figure>& right);

