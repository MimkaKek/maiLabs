#include "TQueue.h"

template <class T> 
TQueue<T>::TQueue() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

template <class T> 
void TQueue<T>::push(T* item) {
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

template <class T> 
bool TQueue<T>::empty() {
    return head == nullptr;
}

template <class T> 
T* TQueue<T>::pop() {
    T* result = nullptr;
    if (head != nullptr) {
        result = head->GetValue();
        head = head->GetNext();
    }
    --size;
    return result;
}

template <class T> 
TQueue<T>::~TQueue() {
}


#include <functional>
template class TQueue<std::function<void(void)>>;
