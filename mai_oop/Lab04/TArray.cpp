#include "TArray.h"
#include <memory>
#include <iostream>
#include <cmath>

template <class T> 
TArray<T>::TArray() {
    Max = 0;
    Current = 0;
    Array = nullptr;
}

template <class T>
bool TArray<T>::Empty() {
    return (Current == 0);
}

template <class T> 
void TArray<T>::Push(std::shared_ptr<T> figure) {

    if(!Array) {
        Array = new std::shared_ptr<TArrayItem<T>>[1];
        Max = 1;
        Current = 1;
        return;
    }
    ++Current;

    if(Current > Max) {
        SetNewSize(Max * 2);
    }
    
    Array[Current - 1]->SetItem(figure);

    if(Current - 2 >= 0) {
        Array[Current - 2]->SetNext(&(**(Array + Current - 1)));
    }

    Array[Current - 1]->SetNext(&End);
    return;
}

template <class T> 
std::shared_ptr<T> TArray<T>::Pop(int i) {

    std::shared_ptr<T> answ = Array[i - 1]->GetItem();

    if(i < 1 || i > Current) {
        return nullptr;
    }

    Array[i - 1] = Array[Current - 1];
    Array[Current - 1] = nullptr;

    --Current;

    if(Current <= Max) {
        SetNewSize(Max / 2);
    }

    return answ;
}

template <class T>
TIterator<TArray<T>,TArrayItem<T>> TArray<T>::begin() {
    return TIterator<TArray<T>,TArrayItem<T>>(*Array);
}

template <class T>
TIterator<TArray<T>,TArrayItem<T>> TArray<T>::end() {
    return TIterator<TArray<T>,TArrayItem<T>>(End);
}

template <class T> 
void TArray<T>::Print(const int step) const {
    Array[step]->GetItem()->Print();
    return;
}

template <class A> 
std::ostream& operator << (std::ostream& os, TArray<A>& Array) {
    for(int step = 0; step < Array.Current; step++) {
        os << step + 1 << ". ";
        Array.Print(step);
    }
    return os;
}

template <class T> 
TArray<T>::~TArray() {
    for(int i = 0; i < Current; i++) {
        Array[i] = nullptr;
    }
    
    delete[] Array;
    std::cout << "Array destroyed!" << std::endl;
}

template <class T> 
void TArray<T>::SetNewSize(unsigned int nSize) {
    
    std::shared_ptr<TArrayItem<T>>* tmp = (nSize == 0) ? nullptr : new std::shared_ptr<TArrayItem<T>>[nSize];

    for(unsigned int i = 0; i < Current; ++i) {
        tmp[i] = Array[i];
    }
    delete[] Array;

    Max = nSize;
    Array = tmp;

    return;
}

#include "Figure.h"
template class TArray<Figure>;
template std::ostream& operator<<(std::ostream& os, TArray<Figure>& array);
