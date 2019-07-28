#include "TArray.h"
#include <memory>
#include <iostream>
#include <cmath>

template <class T> 
TArray<T>::TArray() {
	amount = 0;
	array = nullptr;
}

template <class T> 
void TArray<T>::Add(std::shared_ptr<T> figure) {
	if(amount == 0) {
		array  = new std::shared_ptr<T>;
        amount++;
		*array = figure;
	}
    else {
        std::shared_ptr<T>* tmp = new std::shared_ptr<T>[amount + 1];
        for(int step = 0; step < amount; step++) {
            tmp[step]   = array[step];
            array[step] = nullptr;
        }
        tmp[amount] = figure;
        if(amount == 1) {
            delete array;    
        }
        else {
            delete[] array;
        }
        array = tmp;
        ++amount;
    }
    return;
}

template <class T> 
std::shared_ptr<T> TArray<T>::Remove(int i) {
	
    std::shared_ptr<T>* tmp    = nullptr;
    std::shared_ptr<T>  result = nullptr;
    
	result            = array[i - 1];
	array[i - 1]      = array[amount - 1];
    array[amount - 1] = nullptr;
	
    if(amount != 1) {
        tmp = new std::shared_ptr<T>[amount - 1];
    }
    
    for(int step = 0; step < amount - 1; step++) {
        tmp[step]   = array[step];
        array[step] = nullptr;
    }
    
    if(amount == 1) {
        delete array;    
    }
    else {
        delete[] array;
        array = tmp;
    }
    
    --amount;
    return result;
}

template <class T> 
void TArray<T>::Print(const int step) const {
    array[step]->Print();
	return;
}

template <class A> 
std::ostream& operator << (std::ostream& os, TArray<A>& array) {
	for(int step = 0; step < array.amount; step++) {
		os << step + 1 << ". ";
		array.Print(step);
	}
	return os;
}

template <class T> 
TArray<T>::~TArray() {
    for(int i = 0; i < amount; i++) {
        array[i] = nullptr;
    }
    
    if(amount == 1) {
        delete array;
    }
    else {
        delete[] array;
    }
	
	std::cout << "Array destroyed!" << std::endl;
}

#include "Figure.h"
template class TArray<Figure>;
template std::ostream& operator<<(std::ostream& os, TArray<Figure>& array);
