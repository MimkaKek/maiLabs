#include "TPatriciaTrieItem.h"

//----------------------------------------------------------------------------

TPatriciaTrieItem::TPatriciaTrieItem() {
    Initialize(nullptr, 0, -1, this, this);
}

//----------------------------------------------------------------------------

TPatriciaTrieItem::TPatriciaTrieItem(char* nKey, unsigned long long int nData, int nIndex, TPatriciaTrieItem* nLeft, TPatriciaTrieItem* nRight) {
    Initialize(nKey, nData, nIndex, nLeft, nRight);
}

//----------------------------------------------------------------------------

void TPatriciaTrieItem::Initialize(char* nKey, unsigned long long int nData, int nIndex, TPatriciaTrieItem* nLeft, TPatriciaTrieItem* nRight) {
    
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

TPatriciaTrieItem::~TPatriciaTrieItem() {
    if (key) {
        free(key);
        key = NULL;
    }
}
