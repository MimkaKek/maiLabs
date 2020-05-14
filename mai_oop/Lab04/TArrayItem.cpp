#include "TArrayItem.h"
#include <iostream>

template <class T>
TArrayItem<T>::TArrayItem() {
    this->item = nullptr;
    this->next = nullptr;
    std::cout << "Array item: created" << std::endl;
}

template <class T>
TArrayItem<T>::TArrayItem(const std::shared_ptr<T>& item) {
    this->item = item;
    this->next = nullptr;
    std::cout << "Array item: created" << std::endl;
}

template <class T>
TArrayItem<T>* TArrayItem<T>::GetNext() const {
    return this->next;
}

template <class T>
TArrayItem<T>* TArrayItem<T>::SetNext(TArrayItem<T>* next) {
    TArrayItem<T>* old = this->next;
    this->next = next;
    return old;
}

template <class T>
std::shared_ptr<T> TArrayItem<T>::GetItem() const {
    return this->item;
}

template <class T>
std::shared_ptr<T> TArrayItem<T>::SetItem(std::shared_ptr<T> &item) {
    std::shared_ptr<T> old = this->item;
    this->item = item;
    return old;
}

template <class T>
TArrayItem<T>::~TArrayItem() {
    std::cout << "Array item: deleted" << std::endl;

}

template <class A>
std::ostream& operator << (std::ostream& os, const TArrayItem<A>& obj) {
    os << "[" << *obj.item << "]" << std::endl;
    return os;
}

template <class T>
void * TArrayItem<T>::operator new (size_t size) {
    std::cout << "Allocated: " << size << "bytes" << std::endl;
    return malloc(size);
}

template <class T>
void TArrayItem<T>::operator delete(void *p) {
    std::cout << "Deleted!" << std::endl;
    free(p);
}


#include "Rhombus.h"
template class TArrayItem<Figure>;
template std::ostream& operator<<(std::ostream& os, const TArrayItem<Figure>& obj);
