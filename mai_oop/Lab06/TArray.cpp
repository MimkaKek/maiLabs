#include "TArray.h"
#include <memory>
#include <iostream>
#include <cmath>

template <class T> 
TArray<T>::TArray() {
    amount = 0;
    current = 0;
    array = nullptr;
}

template <class T>
bool TArray<T>::Empty() {
    return amount == 0;
}

template <class T> 
bool TArray<T>::Push(std::shared_ptr<T> &&figure) {
    std::shared_ptr<TQueue<T>> tmp;
    if(figure == nullptr) {
        return true;
    }
    if(amount == 0) {
        ++amount;
        array = (std::shared_ptr<TArrayItem<TQueue<T>>>*) malloc(sizeof(std::shared_ptr<TArrayItem<TQueue<T>>>));
        std::shared_ptr<TArrayItem<TQueue<T>>> smth(new TArrayItem<TQueue<T>>());
        array[current] = smth;
        array[current]->GetValue()->push(figure);
        return true;
    }
    else {
        if(current == amount - 1) {
            amount *= 2;
            array = (std::shared_ptr<TArrayItem<TQueue<T>>>*) realloc(array, amount*sizeof(std::shared_ptr<TArrayItem<TQueue<T>>>));
            if(array == nullptr) {
                puts("Error in realloc!");
                return false;
            }
            std::cout << "Size increased to " << amount << "!" << std::endl;
        }
        int step = 0;
        while(array[step]->GetValue()->GetSize() == 5) {
            ++step;
        }
        array[step]->GetValue()->push(figure);
        if(array[step]->GetValue()->GetSize() == 5) {
            ++current;
            std::shared_ptr<TArrayItem<TQueue<T>>> smth(new TArrayItem<TQueue<T>>);
            array[current] = smth;
        }
        return true;
    }
}

template <class T> 
std::shared_ptr<T> TArray<T>::Pop(int i) {
    std::shared_ptr<T> tmp = nullptr;
    int step = (i - 1) / 5;
    int index = (i - 1) % 5;
    if(array[step]->GetValue()->GetSize() == 1) {
        std::shared_ptr<TArrayItem<TQueue<T>>> smth;
        tmp = array[step]->GetValue()->pop(index);
        if(!tmp) {
            return nullptr;
        }
        if(step != 0) {
            array[step - 1]->SetNext(array[step + 1]);
        }
        for(int i = step; i < current; ++i) {
            smth = array[i + 1];
            array[i + 1] = array[i];
            array[i] = smth;
        }
        --current;
    }
    if(array[step]->GetValue()->GetSize() > 1) {
        tmp = array[step]->GetValue()->pop(index);
    }
    if(current == (amount / 2) - 1) {
        amount /= 2;
        array = (std::shared_ptr<TArrayItem<TQueue<T>>>*) realloc(array, amount * sizeof(std::shared_ptr<TArrayItem<TQueue<T>>>));
        std::cout << "Size decreased to " << amount << "!" << std::endl;
    }
    return tmp;
}
/*
template <class T>
TIterator<TArrayItem<TQueue<T>>,TQueue<T>> TArray<T>::begin() {
    return TIterator<TArrayItem<TQueue<T>>, TQueue<T>>(*array);
}

template <class T>
TIterator<TArrayItem<TQueue<T>>,TQueue<T>> TArray<T>::end() {
    return TIterator<TArrayItem<TQueue<T>>, TQueue<T>>(nullptr);
}
*/
template <class T>
std::shared_ptr<TArrayItem<TQueue<T>>>* TArray<T>::GetArray() {
    if(amount != 0) {
        return array;
    }
    else {
        return nullptr;
    }
}

template <class T> 
std::ostream& TArray<T>::Print(const int step, std::ostream& os) const {
    return array[step]->Print(os);
}

template <class A> 
std::ostream& operator<<(std::ostream& os, TArray<A>& obj) {
    for(int step = 0; step <= obj.current; step++) {
        os << step + 1 << ":\n" << *obj.array[step];
    }
    return os;
}

template <class T> 
TArray<T>::~TArray() {
    for(int i = 0; i < amount; i++) {
        array[i] = nullptr;
    }
    free(array);
    std::cout << "Array destroyed!" << std::endl;
}


#include "Figure.h"
template class TArray<Figure>;
template std::ostream& operator<<(std::ostream& os, TArray<Figure>& array);

