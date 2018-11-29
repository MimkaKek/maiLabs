#include "TQueue.h"

template <typename T> 
TQueue<T>::TQueue() : head(nullptr), tail(nullptr), size(0) {
}


template <typename T> 
std::ostream& operator<<(std::ostream& os, const TQueue<T>& queue) {
    std::shared_ptr<TQueueItem<T>> item = queue.head;
    while(item != nullptr) {
      os << *item;
      item = item->GetNext();
    }
    return os;
}

template <typename T> 
void TQueue<T>::push(std::shared_ptr<T> &&item) {
    std::shared_ptr<TQueueItem<T>> other(new TQueueItem<T>(item));
    if(tail == nullptr) {
        head = other;
        tail = other;
        size = 1;
    }
    else {
        tail->SetNext(other);
        tail = tail->GetNext();
        ++size;
    }
}

template <typename T> 
bool TQueue<T>::empty() {
    return head == nullptr;
}

template <typename T> 
size_t TQueue<T>::GetSize() {
    return size;
}

template <typename T> 
std::shared_ptr<T> TQueue<T>::pop() {
    std::shared_ptr<T> result = nullptr;
    if (head != nullptr) {
        result = head->GetValue();
        head = head->GetNext();
    }
    --size;
    return result;
}

template <typename T> 
TIterator<TQueueItem<T>,T> TQueue<T>::begin() {
    return TIterator<TQueueItem<T>,T>(head);
}

template <typename T> 
TIterator<TQueueItem<T>,T> TQueue<T>::end() {
    return TIterator<TQueueItem<T>,T>(nullptr);
}

template <typename T> 
TQueue<T>::~TQueue() {
}
