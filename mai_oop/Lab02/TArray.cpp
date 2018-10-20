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
		array = (std::shared_ptr<Figure>*) realloc(array, (amount + 1) * sizeof(std::shared_ptr<Figure>));
		/*if(array == nullptr) {
			std::cout << "Ошибка - нехватка памяти!" << std::endl;
			exit(1);
		}*/
		array[amount++] = figure;
		return;
	}
}

void TArray::Remove(std::shared_ptr<Figure> figure) {
	std::shared_ptr<Figure> tmp;
	for(int step = 0; step < amount; step++) {
		if(*figure == *array[step]) {
			for(int push = step; push < (amount - 1); push++) {
				tmp = array[push];
				array[push] = array[push + 1];
				array[push + 1] = tmp;
			}
			array = (std::shared_ptr<Figure>*) realloc(array, (--amount) * sizeof(std::shared_ptr<Figure>));
			break;
		}
	}
	return;
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
  delete array;
	std::cout << "Array destroyed!" << std::endl;
}
