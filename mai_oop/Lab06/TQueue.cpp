#include "TQueue.h"

template <class T> 
TQueue<T>::TQueue() {
    head = nullptr;
    size = 0;
}


template <class T> 
std::ostream& operator<<(std::ostream& os, const TQueue<T>& queue) {
    std::shared_ptr<TQueueItem<T>> item = queue.head;
    while(item != nullptr) {
      os << *item;
      item = item->GetNext();
    }
    return os;
}

template <class T> 
void TQueue<T>::push(std::shared_ptr<T> item) {
    std::shared_ptr<TQueueItem<T>> tmp = head;
    std::shared_ptr<TQueueItem<T>> other(new TQueueItem<T>(item));
    if(head == nullptr) {
        head = other;
    }
    else {
        if(other->GetValue()->Square() <= head->GetValue()->Square()) {
            head = other;
            other->SetNext(tmp);
            ++size;
            return;
        }
        while(other->GetValue()->Square() >= tmp->GetValue()->Square()) {
            tmp = tmp->GetNext();
        }
    }
    ++size;
}

template <class T> 
size_t TQueue<T>::GetSize() {
    return size;
}

template <class T> 
bool TQueue<T>::empty() {
    return head == nullptr;
}

template <class T> 
std::shared_ptr<T> TQueue<T>::pop(int index) {
    
    std::shared_ptr<T> result = nullptr;
    std::shared_ptr<TQueueItem<T>> tmp = head;
    if(index == 0) {
        result = head->GetValue();
        head = head->GetNext();
    }
    else if(index == size - 1) {
        for(int i = 1; i < index; ++i)
            tmp = tmp->GetNext();
        result = tmp->GetNext()->GetValue();
        tmp->SetNext(nullptr);
    }
    else if(index >= 0 && index < size) {
        for(int i = 1; i < index; ++i) {
            tmp = tmp->GetNext();
        }
        result = tmp->GetNext()->GetValue();
        tmp->SetNext(tmp->GetNext()->GetNext());
    }
    else {
        std::cout << "Out of queue borders" << std::endl;
        return nullptr;
    }
    --size;
    return result;
}
/*
template <class T> 
TIterator<TQueueItem<T>,T> TQueue<T>::begin() {
    return TIterator<TQueueItem<T>,T>(head);
}

template <class T> 
TIterator<TQueueItem<T>,T> TQueue<T>::end() {
    return TIterator<TQueueItem<T>,T>(nullptr);
}
*/
template <class T> 
TQueue<T>::~TQueue() {
    head = nullptr;
}

#include "Figure.h"
template class TQueue<Figure>;
template std::ostream& operator<<(std::ostream& os, const TQueue<Figure>& obj);
