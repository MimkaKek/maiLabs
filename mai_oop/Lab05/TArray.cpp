#include "TArray.h"
#include <memory>
#include <iostream>
#include <cmath>

template <class T> 
TArray<T>::TArray() {
    amount = 0;
    current = 0;
    array = NULL;
}

template <class T>
bool TArray<T>::Empty() {
    return amount == 0;
}

template <class T> 
bool TArray<T>::Push(std::shared_ptr<T> &&figure) {
    std::shared_ptr<TArrayItem<T>> other(new TArrayItem<T>(figure));
    
    if(figure == nullptr) {
        return true;
    }
    if(amount == 0) {
        array = (std::shared_ptr<TArrayItem<T>>*) malloc(sizeof(std::shared_ptr<T>));
        amount = 1;
        current = 0;
        array[current] = other;
        return true;
    }
    else {
        if(current == amount - 1) {
            amount *= 2;
            array = (std::shared_ptr<TArrayItem<T>>*) realloc(array, amount*sizeof(std::shared_ptr<T>));
            if(array == nullptr) {
                puts("Error in realloc!");
                return false;
            }
            std::cout << "Size increased to " << amount << "!" << std::endl;
        }
        array[current]->SetNext(other);
        ++current;
        array[current] = other;
        return true;
    }
}

template <class T> 
std::shared_ptr<T> TArray<T>::Pop(int i) {
    
    std::shared_ptr<TArrayItem<T>> tmp;
    if(i > 1 && i < current) {
        array[i - 2]->SetNext(array[i]);
    }
    else if(i >= current) {
        std::cout << "Warning: Index out of array borders" << std::endl; 
        return nullptr;
    }
    for(int step = i - 1; step < current; ++step) {
        tmp = array[step];
        array[step] = array[step + 1];
        array[step + 1] = tmp;
    }
    tmp = array[current];
    array[current] = nullptr;
    --current;
    if(current == (amount / 2) - 1) {
        amount /= 2;
        array = (std::shared_ptr<TArrayItem<T>>*) realloc(array, amount * sizeof(std::shared_ptr<T>));
        std::cout << "Size decreased to " << amount << "!" << std::endl;
    }
    return tmp->GetValue();
}

template <class T>
TIterator<TArrayItem<T>,T> TArray<T>::begin() {
    return TIterator<TArrayItem<T>, T>(*array);
}

template <class T>
TIterator<TArrayItem<T>,T> TArray<T>::end() {
    return TIterator<TArrayItem<T>, T>(nullptr);
}

template <class T>
std::shared_ptr<TArrayItem<T>>* TArray<T>::GetArray() {
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
    for(int step = 0; step < obj.amount; step++) {
        os << step + 1 << ". " << *obj.array[step];
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

