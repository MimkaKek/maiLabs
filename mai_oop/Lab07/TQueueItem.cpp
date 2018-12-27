#include "TQueueItem.h"
#include <iostream>

template <typename T> 
TQueueItem<T>::TQueueItem(const std::shared_ptr<T>& item) {
    this->item = item;
    this->next = nullptr;
    std::cout << "Memory slot created!" << std::endl;
}

template <typename T> 
std::shared_ptr<TQueueItem<T>> TQueueItem<T>::SetNext(std::shared_ptr<TQueueItem<T>> &next) {
    std::shared_ptr<TQueueItem <T>> old = this->next;
    this->next = next;
    return old;
}

template <typename T> 
std::shared_ptr<T> TQueueItem<T>::GetValue() const {
    return this->item;
}

template <typename T> 
std::shared_ptr<TQueueItem<T>> TQueueItem<T>::GetNext() {
    return this->next;
}

template <typename T> 
TQueueItem<T>::~TQueueItem() {
    std::cout << "Memory slot removed!" << std::endl;

}

template <typename A> 
std::ostream& operator<<(std::ostream& os, const TQueueItem<A>& obj) {
    os << *obj.item << std::endl;
    return os;
}

template class TQueueItem<char>;
template std::ostream& operator<<(std::ostream& os, const TQueueItem<char>& obj);
