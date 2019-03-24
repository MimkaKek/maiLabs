#include "TPatriciaTrieItem.h"
#include <cstring>
#include <cstdlib>

//----------------------------------------------------------------------------
template <class T>
TPatriciaTrieItem<T>::TPatriciaTrieItem() {
    Initialize(nullptr, 0, -1, this, this);
}

//----------------------------------------------------------------------------
template <class T>
TPatriciaTrieItem<T>::TPatriciaTrieItem(char* nKey, T nData, int nIndex, TPatriciaTrieItem<T>* nLeft, TPatriciaTrieItem<T>* nRight) {
    Initialize(nKey, nData, nIndex, nLeft, nRight);
}

//----------------------------------------------------------------------------
template <class T>
void TPatriciaTrieItem<T>::Initialize(char* nKey, T nData, int nIndex, TPatriciaTrieItem<T>* nLeft, TPatriciaTrieItem<T>* nRight) {
    
    if (nKey) {
        key = (char*)strdup(nKey);
    }
    else {
        key = nKey;
    }
    
    data      = nData;
    index     = nIndex;
    left      = nLeft;
    right     = nRight;
}

//----------------------------------------------------------------------------
template <class T>
TPatriciaTrieItem<T>::~TPatriciaTrieItem() {
    if (key) {
        free(key);
        key = NULL;
    }
}

template class TPatriciaTrieItem<unsigned long long int>;
