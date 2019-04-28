#include "TPatriciaTrie.h"
#include <stdlib.h>
#include <iostream>
#include <cstring>

const int MAX_DEEP = 10;

//----------------------------------------------------------------------------
template <class T>
TPatriciaTrie<T>::TPatriciaTrie() {
    head = new TPatriciaTrieItem<T>();
    if(!head) {
        std::cout << "ERROR: bad allocation!" << std::endl;
        exit(0);
    }
    head->key = (char*)calloc(256, 1);
    if(!head->key) {
        std::cout << "ERROR: bad allocation!" << std::endl;
        exit(0);
    }
}

//----------------------------------------------------------------------------
template <class T>
TPatriciaTrie<T>::~TPatriciaTrie() {
    RecursiveRemove(head);
}

//----------------------------------------------------------------------------
template <class T>
TPatriciaTrieItem<T>* TPatriciaTrie<T>::Insert(char* key, T data) {
    
    int                     bitIndex;
    TPatriciaTrieItem<T>*   tmp;
    TPatriciaTrieItem<T>*   prev = head;
    TPatriciaTrieItem<T>*   directNode = head->right;
    
    while (prev->index < directNode->index) {
        prev = directNode;
        directNode = (BitGet(key, directNode->index)) ? directNode->right : directNode->left;
    }
    
    
    
    if (KeyCompare(key, directNode->key)) {
        return nullptr;
    }
    
    bitIndex = BitFirstDifferent(key, directNode->key);
    
    prev = head;
    tmp = head->right;
    
    while ( (prev->index < tmp->index) && (tmp->index < bitIndex) ) {
        prev = tmp;
        tmp = BitGet(key, tmp->index) ? tmp->right : tmp->left;
    }
    
    TPatriciaTrieItem<T>* newNode = new TPatriciaTrieItem<T>();
    if(!newNode) {
        std::cout << "ERROR: bad allocation!" << std::endl;
        exit(0);
    }
    
    newNode->Initialize(key, data, bitIndex, BitGet(key, bitIndex) ? tmp : newNode, BitGet(key, bitIndex) ? newNode : tmp);        
    
    if(BitGet(key, prev->index)) {
        prev->right = newNode;
    }
    else {
        prev->left = newNode;
    }
    
    return newNode;
    
}

//----------------------------------------------------------------------------
template <class T>
void TPatriciaTrie<T>::Print(TPatriciaTrieItem<T>* root, int deep) {
    
    int pDeep;
    
    if ( root->index < root->left->index ) {
        Print(root->left, deep + 1);
    }
    else {
        for(int i = 0; i < deep + 1; ++i) {
            std::cout << "|\t";
        }
        std::cout << root->left->data;
        pDeep = deep + 1;
        while(pDeep < MAX_DEEP) {
            ++pDeep;
            std::cout << "\t|";
        }
        std::cout << std::endl;
    }
    
    for(int i = 0; i < deep; ++i) {
        std::cout << "|\t";
    }
    std::cout << root->data << ':' << root->index;
    pDeep = deep;
    while(pDeep < MAX_DEEP) {
        ++pDeep;
        std::cout << "\t|";
    }
    std::cout << std::endl;

    if ( root->index < root->right->index ) {
        Print(root->right, deep + 1);
    }
    else {
        for(int i = 0; i < deep + 1; ++i) {
            std::cout << "|\t";
        }
        std::cout << root->right->data;
        pDeep = deep + 1;
        while(pDeep < MAX_DEEP) {
            ++pDeep;
            std::cout << "\t|";
        }
        std::cout << std::endl;
    }
}

//----------------------------------------------------------------------------
template <class T>
bool TPatriciaTrie<T>::SaveTrieBefore(TPatriciaTrieItem<T>* root, std::ofstream* file) {
    
    int length = 0;
    
    if( head->right == head) {
        return true;
    }
    
    length = strlen(root->key);
    file->write((char*)&length, sizeof(int));
    file->write(root->key, sizeof(char)*length);
    file->write((char*)&root->data, sizeof(T));
    
    if ( root->index < root->left->index ) {
        if(!SaveTrieBefore(root->left, file)) {
            return false;
        }
    }

    if ( root->index < root->right->index ) {
        if(!SaveTrieBefore(root->right, file)) {
            return false;
        }
    }
    
    return true;
}

//----------------------------------------------------------------------------
template <class T>
void TPatriciaTrie<T>::LoadTrieBefore(char* key, std::ifstream* file) {
    
    T data = 0;
    int length = 0;
    
    while(!file->eof()) {
        file->read((char*)&length,sizeof(int));
        file->read(key, length*sizeof(char));
        key[length] = '\0';
        file->read((char*)&data,sizeof(T));
        
        Insert(key, data);
    }
    
}

//----------------------------------------------------------------------------

template <class T>
bool TPatriciaTrie<T>::SaveTrieCurrent(TPatriciaTrieItem<T>* root, std::ofstream* file) {
    
    TPack   pack;
    char    check;
    
    if( head->right == head ) {
        check = 1;
        file->write(&check, sizeof(char));
        return true;
    }
    else if( head == root ) {
        check = 0;
        pack.up = 0;
        pack.length = strlen(root->right->key);
        pack.index = root->right->index;
        pack.data = root->right->data;
        file->write(&check, sizeof(char));
        file->write((char*)&pack, sizeof(TPack));
        file->write(root->right->key, sizeof(char)*pack.length);
        return SaveTrieCurrent(root->right, file);
    }
    
    pack.up = ( root->index < root->left->index ) ? 0 : 1;
    pack.length = strlen(root->left->key);
    if(!pack.up) {
        pack.index = root->left->index;
        pack.data = root->left->data;
    }
    file->write((char*)&pack, sizeof(TPack));
    file->write(root->left->key, sizeof(char)*pack.length);
    
    pack.up = ( root->index < root->right->index ) ? 0 : 1;
    pack.length = strlen(root->right->key);
    if(!pack.up) {
        pack.index = root->right->index;
        pack.data = root->right->data;
    }
    file->write((char*)&pack, sizeof(TPack));
    file->write(root->right->key, sizeof(char)*pack.length);
    
    if ( root->index < root->left->index ) {
        SaveTrieCurrent(root->left, file);
    }

    if ( root->index < root->right->index ) {
        SaveTrieCurrent(root->right, file);
    }
    
    return true;
}

//----------------------------------------------------------------------------
template<class T>
void TPatriciaTrie<T>::LoadTrieCurrent(char* key, TPatriciaTrieItem<T>* root, std::ifstream* file) {
    
    TPack   pack;
    char    check;
    
    if( head == root ) {
        file->read(&check, sizeof(char));
        if(check) {
            return;
        }
        file->read((char*)&pack,sizeof(TPack));
        file->read(key, pack.length*sizeof(char));
        key[pack.length] = '\0';
        TPatriciaTrieItem<T>* newNode = new TPatriciaTrieItem<T>();
        if(!newNode) {
            std::cout << "ERROR: bad allocation!" << std::endl;
            exit(0);
        }
        newNode->Initialize(key, pack.data, pack.index, newNode, newNode);
        head->right = newNode;
        return LoadTrieCurrent(key, head->right, file);
    }
    else {
        file->read((char*)&pack,sizeof(TPack));
        file->read(key, pack.length*sizeof(char));
        key[pack.length] = '\0';
        if(!pack.up) {
            TPatriciaTrieItem<T>* newNode = new TPatriciaTrieItem<T>();
            if(!newNode) {
                std::cout << "ERROR: bad allocation!" << std::endl;
                exit(0);
            }
            newNode->Initialize(key, pack.data, pack.index, newNode, newNode);
            root->left = newNode;
        }
        else {
            TPatriciaTrieItem<T>* current = head;
            while(strcmp(current->key, key)) {
                current = (BitGet(key, current->index)) ? current->right : current->left;
            }
            root->left = current;
        }
        
        file->read((char*)&pack,sizeof(TPack));
        file->read(key, pack.length*sizeof(char));
        key[pack.length] = '\0';
        if(!pack.up) {
            TPatriciaTrieItem<T>* newNode = new TPatriciaTrieItem<T>();
            if(!newNode) {
                std::cout << "ERROR: bad allocation!" << std::endl;
                exit(0);
            }
            newNode->Initialize(key, pack.data, pack.index, newNode, newNode);
            root->right = newNode;
        }
        else {
            TPatriciaTrieItem<T>* current = head;
            while(strcmp(current->key, key)) {
                current = (BitGet(key, current->index)) ? current->right : current->left;
            }
            root->right = current;
        }
        
        if ( root->index < root->left->index ) {
            LoadTrieCurrent(key, root->left, file);
        }

        if ( root->index < root->right->index ) {
            LoadTrieCurrent(key, root->right, file);
        }
        
        return;
    }
}

//----------------------------------------------------------------------------
template <class T>
T* TPatriciaTrie<T>::Lookup(char* k) {

    TPatriciaTrieItem<T>* node = LookupNode(k);

    if (!node)
        return nullptr;

    return &(node->data);

}

//----------------------------------------------------------------------------
template <class T>
TPatriciaTrieItem<T>* TPatriciaTrie<T>::LookupNode(char* key) {
    
    TPatriciaTrieItem<T>*   prev = head;
    TPatriciaTrieItem<T>*   directNode = head->right;
    
    while( prev->index < directNode->index ) {
        prev = directNode;
        directNode = BitGet(key, directNode->index) ? directNode->right : directNode->left;
    }
    
    if (!KeyCompare(key, directNode->key)) {
        return nullptr;
    }

    return directNode;

}

//----------------------------------------------------------------------------
template <class T>
bool TPatriciaTrie<T>::Delete(char* key) {
    
    TPatriciaTrieItem<T> *path[2], *directNode, *x, *lp;

    path[1]     = head;
    directNode  = head->right;

    while (path[1]->index < directNode->index) {
        path[0]     = path[1];
        path[1]     = directNode;
        directNode  = BitGet(key, directNode->index) ? directNode->right : directNode->left;
    }
    
    if (!KeyCompare(key, directNode->key)) {
        return false;
    }

    if (directNode != path[1])
        KeyCopy(path[1], directNode);

    if ((path[1]->left->index > path[1]->index) || (path[1]->right->index > path[1]->index)) {

        if (path[1] != directNode) {

            lp = path[1];
            x  = BitGet(path[1]->key, path[1]->index) ? path[1]->right : path[1]->left;
 
            while (lp->index < x->index) {
                lp = x;
                x  = BitGet(path[1]->key, x->index) ? x->right : x->left;
            }

            if (!KeyCompare(path[1]->key, x->key)) {
                return false;
            }

            if (BitGet(path[1]->key, lp->index)) {
                lp->right = directNode;
            }
            else {
                lp->left  = directNode;
            }
        }

        if (path[0] != path[1]) {
            TPatriciaTrieItem<T>* child = BitGet(key, path[1]->index) ? path[1]->left : path[1]->right;
            if (BitGet(key, path[0]->index)) {
                path[0]->right = child;
            }
            else {
                path[0]->left  = child;
            }
        }
   
    }
    else if (path[0] != path[1]) {
        TPatriciaTrieItem<T>* cLeft = path[1]->left;
        TPatriciaTrieItem<T>* cRight = path[1]->right;
        if (BitGet(key, path[0]->index)) {
            path[0]->right = (((cLeft == cRight) && (cLeft == path[1])) ? path[0] : ((cLeft==path[1]) ? cRight : cLeft ));
        }
        else {
            path[0]->left  = (((cLeft == cRight) && (cLeft == path[1])) ? path[0] : ((cLeft==path[1]) ? cRight : cLeft ));
        }
    }

    delete path[1];

    return true;
}

//----------------------------------------------------------------------------
template <class T>
void TPatriciaTrie<T>::RecursiveRemove(TPatriciaTrieItem<T>* root) {

    TPatriciaTrieItem<T>* l = root->left;
    TPatriciaTrieItem<T>* r = root->right;

    if ( (l->index >= root->index) && (l != root) && (l != head) )
        RecursiveRemove(l);

    if ( (r->index >= root->index) && (r != root) && (r != head) )
        RecursiveRemove(r);

    delete root;

}

//----------------------------------------------------------------------------
template <class T>
void TPatriciaTrie<T>::ClearTrie() {
    RecursiveRemove(head->right);
    head->right = head;
    return;
}

//----------------------------------------------------------------------------
template <class T>
bool TPatriciaTrie<T>::Empty() {
    if( head->right == head ) {
        return true;
    }
    else {
        return false;
    }
}
//----------------------------------------------------------------------------
template <class T>
int TPatriciaTrie<T>::BitGet(char* bit_stream, int n) {
    if (n == -1) {
        return 2;
    }
    int k = (n & 0x7);
    return ( (*(bit_stream + (n >> 3))) >> k) & 0x1;
}

//----------------------------------------------------------------------------
template <class T>
bool TPatriciaTrie<T>::KeyCompare(char* key1, char* key2) {
    if (!key1 || !key2) {
        return false;
    }
    return (strcmp((char*)key1, (char*)key2) == 0);
}

//----------------------------------------------------------------------------
template <class T>
int TPatriciaTrie<T>::BitFirstDifferent(char* key1, char* key2) {
    
    int         posChar = 0;
    int         posBit = 0;
    
    if (!key1 || !key2) {
        return 0;
    }
    
    while ( (key1[posChar] == key2[posChar]) && (key1[posChar] != 0) && (key2[posChar] != 0) ) {
        posChar++;
    }
    while (BitGet(&key1[posChar], posBit) == BitGet(&key2[posChar], posBit)) {
        posBit++;
    }
    
    return ((posChar << 3) + posBit);
}

//----------------------------------------------------------------------------
template <class T>
TPatriciaTrieItem<T>* TPatriciaTrie<T>::GetHead() {
    return head;
}

//----------------------------------------------------------------------------
template <class T>
void TPatriciaTrie<T>::SwapHead(TPatriciaTrie<T>* nTree) {
    
    TPatriciaTrieItem<T>* tmp;
    
    if( head->right != head ) {
        if( nTree->head->right != nTree->head) {
            tmp = nTree->head->right;
            nTree->head->right = head->right;
            head->right = tmp;
        }
        else {
            tmp = head->right;
            head->right = head;
            nTree->head->right = tmp;
        }
    }
    else {
        if( nTree->head->right != nTree->head) {
            tmp = nTree->head->right;
            nTree->head->right = nTree->head;
            head->right = tmp;
        }
    }
    
    return;
}

//----------------------------------------------------------------------------
template <class T>
void TPatriciaTrie<T>::KeyCopy(TPatriciaTrieItem<T>* src, TPatriciaTrieItem<T>* dest) {

    if (src == dest) {
        return;
    }

    if (strlen(dest->key) < strlen(src->key)) {
        dest->key = (char*)realloc(dest->key, 1 + strlen(src->key));
    }
    
    strcpy(dest->key, src->key);

    dest->data = src->data;
    
}

template class TPatriciaTrie<unsigned long long int>;
