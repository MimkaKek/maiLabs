#include "TArray.h"
#include <memory>
#include <iostream>
#include <cmath>

template <class T> 
TArray<T>::TArray() {
	amount = 0;
	array = NULL;
}

template <class T> 
void TArray<T>::Add(std::shared_ptr<T> figure) {
	if(amount == 0) {
		array = (std::shared_ptr<T>*) malloc((++amount) * sizeof(std::shared_ptr<T>));
		array[0] = figure;
		return;
	}
	else {
		array = (std::shared_ptr<T>*) realloc(array, (++amount) * sizeof(std::shared_ptr<T>));
		array[amount - 1] = figure;
		return;
	}
}

template <class T> 
std::shared_ptr<T> TArray<T>::Remove(int i) {
	std::shared_ptr<T> tmp;
	tmp = array[i - 1];
	array[i - 1] = array[amount - 1];
  array[amount - 1] = nullptr;
	array = (std::shared_ptr<T>*) realloc(array, (--amount) * sizeof(std::shared_ptr<T>));
	return tmp;
}

template <class T> 
unsigned int TArray<T>::GetAmount() const {
	return amount;
}

template <class T> 
void TArray<T>::Print(const int step) {
	array[step]->Print();
	return;
}

template <class A> 
std::ostream& operator<<(std::ostream& os, const TArray<A>& array) {
	for(int step = 0; step < array.GetAmount(); step++) {
		os << step + 1 << ". ";
		array.Print(step);
	}
	return os;
}

template <class T> 
TArray<T>::~TArray() {
  for(int i = 0; i < amount; i++)
    array[i] = nullptr;
	delete array;
	std::cout << "Array destroyed!" << std::endl;
}

#include "Figure.h"
template class TArray<Figure>;
template std::ostream& operator<<(std::ostream& os, const TArray<Figure>& array);
