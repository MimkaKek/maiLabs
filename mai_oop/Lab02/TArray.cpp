#include "TArray.h"
#include <iostream>
#include <cmath>

TArray::TArray() {
	amount = 0;
	array = NULL;
}

void TArray::Add(std::shared_ptr<Figure> figure) {
	if(amount == 0) {
		array = (std::shared_ptr<Figure>*) malloc((++amount) * sizeof(std::shared_ptr<Figure>));
		array[0] = figure;
		return;
	}
	else {
		array = (std::shared_ptr<Figure>*) realloc(array, (++amount) * sizeof(std::shared_ptr<Figure>));
		array[amount - 1] = figure;
		return;
	}
}

std::shared_ptr<Figure> TArray::Remove(int i) {
	std::shared_ptr<Figure> tmp;
	tmp = array[i - 1];
	array[i - 1] = array[amount - 1];
  array[amount - 1] = nullptr;
	array = (std::shared_ptr<Figure>*) realloc(array, (--amount) * sizeof(std::shared_ptr<Figure>));
	return tmp;
}

unsigned int TArray::GetAmount() const {
	return amount;
}

void TArray::Print(int step) {
	array[step]->Print();
	return;
}

std::ostream& operator<<(std::ostream& os, TArray& array) {
	for(unsigned int step = 0; step < array.GetAmount(); step++) {
		os << step + 1 << ". ";
		array.Print(step);
	}
	return os;
}
TArray::~TArray() {
  for(int i = 0; i < amount; i++)
    array[i] = nullptr;
	delete array;
	std::cout << "Array destroyed!" << std::endl;
}
