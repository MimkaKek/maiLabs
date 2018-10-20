#include "TArray.h"
#include <iostream>
#include <cmath>

TArray::TArray() {
	amount = 0;
	array = NULL;
}

void TArray::Add(Rhombus rhombus) {
	if(amount == 0) {
		array = new Rhombus(rhombus);
		if(array == nullptr) {
			std::cout << "Ошибка - нехватка памяти!" << std::endl;
			exit(1);
		}
		amount++;
		return;
	}
	else {
		array = (Rhombus*) realloc(array, (amount + 1) * sizeof(Rhombus));
		/*if(array == nullptr) {
			std::cout << "Ошибка - нехватка памяти!" << std::endl;
			exit(1);
		}*/
		array[amount++] = rhombus;
		return;
	}
}

void TArray::Remove(Rhombus rhombus) {
	Rhombus tmp;
	for(unsigned int step = 0; step < amount; step++) {
		if(rhombus == array[step]) {
			for(unsigned int push = step; push < (amount - 1); push++) {
				tmp = array[push];
				array[push] = array[push + 1];
				array[push + 1] = tmp;
			}
			array = (Rhombus*) realloc(array, (--amount) * sizeof(Rhombus));
			break;
		}
	}
	return;
}

unsigned int TArray::GetAmount() const {
	return amount;
}

Rhombus* TArray::GetRhombus(int pos) const {
	return &array[pos];
}

std::ostream& operator<<(std::ostream& os, TArray& array) {
	for(unsigned int step = 0; step < array.GetAmount(); step++) {
		os << step + 1 << ". " << *array.GetRhombus(step);
	}
	return os;
}
TArray::~TArray() {
  delete array;
	std::cout << "Array destroyed!" << std::endl;
}
