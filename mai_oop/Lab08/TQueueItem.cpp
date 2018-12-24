#include "TQueueItem.h"
#include <iostream>

template <class T> 
TQueueItem<T>::TQueueItem(T* item) {
    this->item = item;
    this->next = nullptr;
    std::cout << "Queue: item created!" << std::endl;
}

template <class T> 
std::shared_ptr<TQueueItem<T>> TQueueItem<T>::SetNext(std::shared_ptr<TQueueItem<T>> &next) {
    std::shared_ptr<TQueueItem <T>> old = this->next;
    this->next = next;
    return old;
}

template <class T> 
T* TQueueItem<T>::GetValue() const {
    return this->item;
}

template <class T> 
std::shared_ptr<TQueueItem<T>> TQueueItem<T>::GetNext() {
    return this->next;
}

template <class T> 
TQueueItem<T>::~TQueueItem() {
    std::cout << "Queue: item removed!" << std::endl;

}

#include <functional>
template class TQueueItem<std::function<void(void)>>;
