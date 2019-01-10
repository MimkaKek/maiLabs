#include "TArray.h"

template <class T> 
TArray<T>::TArray() {
    amount = 0;
    current = 0;
    array = nullptr;
}

template <class T>
bool TArray<T>::Empty() {
    return current == 0;
}

template <class T>
void TArray<T>::Sort() {
    int l, r;
    for(l = 0; l < current - 1; ++l) {
        r = l + 1;
        while(r > 1 && (*array[r - 1]) > (*array[r])) {
            std::shared_ptr<TArrayItem<T>> tmp = array[r - 1];
            array[r - 1] = array[r];
            array[r] = tmp;
            --r;
        }
    }
    return;
}

template <class T>
size_t TArray<T>::Size() {
    return current;
}

template <class T>
void TArray<T>::Reverse() {
    if(current != 0) {
        std::shared_ptr<TArrayItem<T>> tmp = nullptr;
        for(int i = 0; i <= ((current - 1) / 2); ++i) {
            tmp = array[i];
            array[i] = array[current - 1 - i];
            array[current - 1 - i] = tmp;
        }
    }
}

template <class T> 
bool TArray<T>::Push(std::shared_ptr<T> figure) {
    std::shared_ptr<TArrayItem<T>> other(new TArrayItem<T>(figure));
    
    if(figure == nullptr) {
        return false;
    }
    if(amount == 0) {
        array = (std::shared_ptr<TArrayItem<T>>*) malloc(sizeof(std::shared_ptr<TArrayItem<T>>)*22);
        amount = 22;
        current = 1;
        array[current - 1] = other;
        return true;
    }
    else {
        if(current == amount) {
            amount *= 2;
            array = (std::shared_ptr<TArrayItem<T>>*) realloc(array, amount*sizeof(std::shared_ptr<TArrayItem<T>>));
            if(array == nullptr) {
                puts("Error in realloc!");
                return false;
            }
            std::cout << "Size increased to " << amount << "!" << std::endl;
        }
        ++current;
        array[current - 1] = other;
        return true;
    }
}

template <class T> 
std::shared_ptr<T> TArray<T>::Pop(int i) {
    
    std::shared_ptr<TArrayItem<T>> tmp;
    if(i < 0 || i >= current) {
        std::cout << "Warning: out of array!" << std::endl;
        return nullptr;
    }
    for(int step = i; step < current - 1; ++step) {
        tmp = array[step];
        array[step] = array[step + 1];
        array[step + 1] = tmp;
    }
    tmp = array[current - 1];
    array[current - 1] = nullptr;
    --current;
    /*if(current == (amount / 2) - 1) {
        amount /= 2;
        array = (std::shared_ptr<TArrayItem<T>>*) realloc(array, amount * sizeof(std::shared_ptr<T>));
        std::cout << "Size decreased to " << amount << "!" << std::endl;
    }*/
    return tmp->GetValue();
}

template <class T>
TIterator<TArrayItem<T>,T> TArray<T>::begin() {
    return TIterator<TArrayItem<T>, T>(array);
}

template <class T>
TIterator<TArrayItem<T>,T> TArray<T>::end() {
    return TIterator<TArrayItem<T>, T>(array + current);
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
    if(!obj.current) {
        os << "Array is Empty!";
        return os;
    }
    for(int step = 0; step < obj.current; step++) {
        os << step + 1 << ". " << *obj.array[step];
    }
    return os;
}

template <class T> 
TArray<T>::~TArray() {
    for(int i = 0; i < current; i++) {
        array[i] = nullptr;
    }
    free(array);
    std::cout << "Array destroyed!" << std::endl;
}


#include "Figure.h"
template class TArray<Figure>;
template std::ostream& operator<<(std::ostream& os, TArray<Figure>& array);

