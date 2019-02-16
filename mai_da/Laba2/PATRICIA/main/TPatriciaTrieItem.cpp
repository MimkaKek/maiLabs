#include "TPatriciaTrieItem.h"
#include <cstring>
#include <cstdlib>

//----------------------------------------------------------------------------
template <class T>
TPatriciaTrieItem<T>::TPatriciaTrieItem() {
    Initialize(NULL, 0, -1, this, this);
}

//----------------------------------------------------------------------------
template <class T>
TPatriciaTrieItem<T>::TPatriciaTrieItem(char* k, T d, int bi, TPatriciaTrieItem<T>* l, TPatriciaTrieItem<T>* r) {
    Initialize(k, d, bi, l, r);
}

//----------------------------------------------------------------------------
template <class T>
void TPatriciaTrieItem<T>::Initialize(char* k, T d, int bi, TPatriciaTrieItem<T>* l, TPatriciaTrieItem<T>* r) {
    
    if (k) {
        key = (char*)strdup(k);
    }
    else {
        key = k;
    }
    
    data      = d;
    index     = bi;
    left      = l;
    right     = r;
}

//----------------------------------------------------------------------------
template <class T>
TPatriciaTrieItem<T>::~TPatriciaTrieItem() {
    if (key) {
        free(key);
        key = NULL;
    }
}

//----------------------------------------------------------------------------
template <class T>
T TPatriciaTrieItem<T>::GetData() {
    return data;
}

//----------------------------------------------------------------------------
template <class T>
bool TPatriciaTrieItem<T>::SetData(T d) {
    memcpy(&data, &d, sizeof(T));
    return true;
}

//----------------------------------------------------------------------------
template <class T>
char* TPatriciaTrieItem<T>::GetKey() {
    return key;
}

//----------------------------------------------------------------------------
template <class T>
TPatriciaTrieItem<T>* TPatriciaTrieItem<T>::GetLeft() {
    return left;
}

//----------------------------------------------------------------------------
template <class T>
TPatriciaTrieItem<T>* TPatriciaTrieItem<T>::GetRight() {
    return right;
}

template class TPatriciaTrieItem<unsigned long long int>;
