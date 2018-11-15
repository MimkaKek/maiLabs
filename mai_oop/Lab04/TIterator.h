#ifndef TITERATOR_H
#define	TITERATOR_H
#include <memory>
#include <iostream>

template <class node, class T>
class TIterator {
public:
    TIterator(std::shared_ptr<T>* n)   {
        node_ptr = n;
    }
    T* operator * () {
        return &(**node_ptr);
    }
    T* operator -> () {
        return &(**node_ptr);
    }
    void operator ++ () {
        ++node_ptr;
    }
    TIterator operator ++ (int) {
        TIterator iter(*this);
        ++(*this);
        return iter;
    }
    bool operator == (TIterator const& i) {
        return node_ptr == i.node_ptr;
    }
    bool operator != (TIterator const& i) {
        return !(*this == i);
    }
private:
    std::shared_ptr<T>* node_ptr;
};

#endif
