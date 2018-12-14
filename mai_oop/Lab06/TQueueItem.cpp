#include "TQueueItem.h"
#include <iostream>

template <class T> 
TQueueItem<T>::TQueueItem(const std::shared_ptr<T>& item) {
    this->item = item;
    this->next = nullptr;
    std::cout << "Stack item: created" << std::endl;
}

template <class T> 
std::shared_ptr<TQueueItem<T>> TQueueItem<T>::SetNext(std::shared_ptr<TQueueItem<T>> next) {
    std::shared_ptr<TQueueItem <T>> old = this->next;
    this->next = next;
    return old;
}

template <class T> 
std::shared_ptr<T> TQueueItem<T>::GetValue() const {
    return this->item;
}

template <class T> 
std::shared_ptr<TQueueItem<T>> TQueueItem<T>::GetNext() {
    return this->next;
}

template <class T> 
TQueueItem<T>::~TQueueItem() {
    next = nullptr;
    std::cout << "Memory get" << std::endl;
}

template <class A> 
std::ostream& operator<<(std::ostream& os, const TQueueItem<A>& obj) {
    os << *obj.item << std::endl;
    return os;
}

#include "Figure.h"
template class TQueueItem<Figure>;
template std::ostream& operator<<(std::ostream& os, const TQueueItem<Figure>& obj);
