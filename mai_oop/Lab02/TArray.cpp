#include "TArray.h"
#include <iostream>
#include <cmath>

TArray::TArray() {
	amount = 0;
	array = nullptr;
}

void TArray::Add(std::shared_ptr<Figure> figure) {
	
    if(amount == 0) {
		array  = new std::shared_ptr<Figure>;
		*array = figure;
	}
    else {
        std::shared_ptr<Figure>* tmp = new std::shared_ptr<Figure>[amount + 1];
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
    }
    
    ++amount;
    return;
}

std::shared_ptr<Figure> TArray::Remove(int i) {
	
    
    std::shared_ptr<Figure>* tmp    = nullptr;
    std::shared_ptr<Figure>  result = nullptr;
    
	if((i < 1 || i > amount) || !amount) {
        return result;
    }
    
    result            = array[i - 1];
	array[i - 1]      = array[amount - 1];
    array[amount - 1] = nullptr;
	
    if(amount > 1) {
        
        if(amount == 2) {
            tmp = new std::shared_ptr<Figure>;
        }
        else {
            tmp = new std::shared_ptr<Figure>[amount - 1];
        }
        
        for(int step = 0; step < amount - 1; step++) {
            tmp[step]   = array[step];
            array[step] = nullptr;
        }
        delete[] array;
        array = tmp;
    }
    else {
        delete array;
        array = nullptr;
    }
    
    --amount;
    return result;
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
    
    if(amount) {
        for(int i = 0; i < amount; i++) {
            array[i] = nullptr;
        }
        
        if(amount == 1) {
            delete array;
        }
        else {
            delete[] array;
        }
    }
	
	std::cout << "Array destroyed!" << std::endl;

}
