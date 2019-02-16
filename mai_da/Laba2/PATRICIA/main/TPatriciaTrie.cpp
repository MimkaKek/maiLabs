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
    TPatriciaTrieItem<T>*   directNode = prev->right;
    
    while (prev->index < directNode->index) {
        prev = directNode;
        directNode = (BitGet(key, directNode->index)) ? directNode->right : directNode->left;
    }
    
    if (KeyCompare(key, directNode->key)) {
        return nullptr;
    }
    
    bitIndex = BitFirstDifferent(key, directNode->key);
    
    prev = head;
    tmp = prev->right;
    
    while ( (prev->index < tmp->index) && (tmp->index < bitIndex) ) {
        prev = tmp;
        tmp = BitGet(key, tmp->index) ? tmp->right : tmp->left;
    }
    
    TPatriciaTrieItem<T>* newNode = new TPatriciaTrieItem<T>();
    if(!newNode) {
        std::cout << "ERROR: bad allocation!" << std::endl;
        exit(0);
    }
    
    if(tmp->index == -1) {
        newNode->Initialize(key, data, bitIndex, newNode, newNode);
    }
    else {
        newNode->Initialize(key, data, bitIndex, BitGet(key, bitIndex) ? tmp : newNode, BitGet(key, bitIndex) ? newNode : tmp);
    }
    
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
    
    if ( root->index < root->left->index || ( root->left->index == 0 && root->left != head->right && root->left != root ) ) {
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

    if ( root->index < root->right->index || ( root->right->index == 0 && root->right != head->right && root->right != root ) ) {
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
bool TPatriciaTrie<T>::SaveTrie(TPatriciaTrieItem<T>* root, std::ofstream* file) {
    
    int length = 0;
    char up = 0;
    
    if( head->right == head) {
        return true;
    }
    else if( head == root ) {
        up = 0;
        file->write((char*)&up, sizeof(char));
        length = strlen(root->right->key);
        file->write((char*)&length, sizeof(int));
        file->write(root->right->key, sizeof(char)*length);
        file->write((char*)&root->right->data, sizeof(unsigned long long int));
        file->write((char*)&root->right->index, sizeof(int));
        return SaveTrie(root->right, file);
    }
    
    up = ( root->index < root->left->index || ( root->left->index == 0 && root->left != head->right && root->left != root ) ) ? 0 : 1;
    file->write((char*)&up, sizeof(char));
    length = strlen(root->left->key);
    file->write((char*)&length, sizeof(int));
    file->write(root->left->key, sizeof(char)*length);
    if(!up) {
        file->write((char*)&root->left->data, sizeof(T));
        file->write((char*)&root->left->index, sizeof(int));
    }
    
    up = ( root->index < root->right->index || ( root->right->index == 0 && root->right != head->right && root->right != root ) ) ? 0 : 1;
    file->write((char*)&up, sizeof(char));
    length = strlen(root->right->key);
    file->write((char*)&length, sizeof(int));
    file->write(root->right->key, sizeof(char)*length);
    if(!up) {
        file->write((char*)&root->right->data, sizeof(T));
        file->write((char*)&root->right->index, sizeof(int));
    }
    
    if ( root->index < root->left->index || ( root->left->index == 0 && root->left != head->right && root->left != root ) ) {
        if(!SaveTrie(root->left, file)) {
            return false;
        }
    }

    if ( root->index < root->right->index || ( root->right->index == 0 && root->right != head->right && root->right != root ) ) {
        if(!SaveTrie(root->right, file)) {
            return false;
        }
    }
    
    return true;
}

//----------------------------------------------------------------------------
template <class T>
void TPatriciaTrie<T>::LoadTrie(char* key, TPatriciaTrieItem<T>* root, std::ifstream* file) {
    
    int length = 0;
    int index = 0;
    T data = 0;
    char up = 0;
    
    if( head == root ) {
        file->read((char*)&up,sizeof(char));
        file->read((char*)&length,sizeof(int));
        file->read(key, length*sizeof(char));
        key[length] = '\0';
        file->read((char*)&data,sizeof(T));
        file->read((char*)&index,sizeof(int));
        TPatriciaTrieItem<T>* newNode = new TPatriciaTrieItem<T>();
        if(!newNode) {
            std::cout << "ERROR: bad allocation!" << std::endl;
            exit(0);
        }
        newNode->Initialize(key, data, index, newNode, newNode);
        head->right = newNode;
        LoadTrie(key, head->right, file);
        return;
    }
    else {
        file->read((char*)&up,sizeof(char));
        file->read((char*)&length,sizeof(int));
        file->read(key, length*sizeof(char));
        key[length] = '\0';
        if(!up) {
            file->read((char*)&data,sizeof(T));
            file->read((char*)&index,sizeof(int));
            TPatriciaTrieItem<T>* newNode = new TPatriciaTrieItem<T>();
            if(!newNode) {
                std::cout << "ERROR: bad allocation!" << std::endl;
                exit(0);
            }
            newNode->Initialize(key, data, index, newNode, newNode);
            root->left = newNode;
        }
        else {
            TPatriciaTrieItem<T>* tmp = head->right;
            while(tmp != root && strcmp(tmp->key, key) != 0) {
                tmp = (BitGet(root->key, tmp->index)) ? tmp->right : tmp->left;
            }
            root->left = tmp;
        }
        file->read((char*)&up,sizeof(char));
        file->read((char*)&length,sizeof(int));
        file->read(key, length*sizeof(char));
        key[length] = '\0';
        if(!up) {
            file->read((char*)&data,sizeof(T));
            file->read((char*)&index,sizeof(int));
            TPatriciaTrieItem<T>* newNode = new TPatriciaTrieItem<T>();
            if(!newNode) {
                std::cout << "ERROR: bad allocation!" << std::endl;
                exit(0);
            }
            newNode->Initialize(key, data, index, newNode, newNode);
            root->right = newNode;
        }
        else {
            TPatriciaTrieItem<T>* tmp = head->right;
            while(tmp != root && strcmp(tmp->key, key) != 0) {
                tmp = (BitGet(root->key, tmp->index)) ? tmp->right : tmp->left;
            }
            root->right = tmp;
        }
        
        if ( root->index < root->left->index ) {
            LoadTrie(key, root->left, file);
        }

        if ( root->index < root->right->index ) {
            LoadTrie(key, root->right, file);
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
void TPatriciaTrie<T>::KeyCopy(TPatriciaTrieItem<T>* src, TPatriciaTrieItem<T>* dest) {

    if (src == dest)
        return;

    // Copy the key from src to dest
    if (strlen(dest->key) < strlen(src->key)) {
        dest->key = (char*)realloc(dest->key, 1 + strlen(src->key));
    }
    
    strcpy(dest->key, src->key);

    // Copy the data from src to dest
    dest->data = src->data;

    // How about the bit index?
    //dest->index = src->index;
    
}

template class TPatriciaTrie<unsigned long long int>;
