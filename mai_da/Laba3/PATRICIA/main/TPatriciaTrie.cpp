#include "TPatriciaTrie.h"
#include <iostream>
#include <cstring>

const int MAX_DEEP = 10;

//----------------------------------------------------------------------------

TPatriciaTrie::TPatriciaTrie() {
    head = new TPatriciaTrieItem();
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

TPatriciaTrie::~TPatriciaTrie() {
    RecursiveRemove(head);
}

//----------------------------------------------------------------------------

TPatriciaTrieItem* TPatriciaTrie::Insert(char* key, unsigned long long int data) {
    
    int                     bitIndex;
    TPatriciaTrieItem*   tmp;
    TPatriciaTrieItem*   prev = head;
    TPatriciaTrieItem*   directNode = head->right;
    
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
    
    TPatriciaTrieItem* newNode = new TPatriciaTrieItem();
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

void TPatriciaTrie::Print(TPatriciaTrieItem* root, int deep) {
    
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

bool TPatriciaTrie::SaveTrieCurrent(TPatriciaTrieItem* root, std::ofstream* file) {
    
    int length = 0;
    
    if( head->right == head) {
        return true;
    }
    
    length = strlen(root->key);
    file->write((char*)&length, sizeof(int));
    file->write(root->key, sizeof(char)*length);
    file->write((char*)&root->data, sizeof(unsigned long long int));
    
    if ( root->index < root->left->index ) {
        if(!SaveTrieCurrent(root->left, file)) {
            return false;
        }
    }

    if ( root->index < root->right->index ) {
        if(!SaveTrieCurrent(root->right, file)) {
            return false;
        }
    }
    
    return true;
}

//----------------------------------------------------------------------------

void TPatriciaTrie::LoadTrieCurrent(char* key, std::ifstream* file) {
    
    unsigned long long int  data = 0;
    int                     length = 0;
    
    while(!file->eof()) {
        file->read((char*)&length,sizeof(int));
        file->read(key, length*sizeof(char));
        key[length] = '\0';
        file->read((char*)&data,sizeof(unsigned long long int));
        
        Insert(key, data);
    }
    
}

//----------------------------------------------------------------------------

bool TPatriciaTrie::SaveTrieBefore(TPatriciaTrieItem* root, std::ofstream* file) {
    
    TPack pack;
    
    if( head->right == head) {
        return true;
    }
    else if( head == root ) {
        pack.up = 0;
        pack.length = strlen(root->right->key);
        pack.index = root->right->index;
        pack.data = root->right->data;
        file->write((char*)&pack, sizeof(TPack));
        file->write(root->right->key, sizeof(char)*pack.length);
        return SaveTrieBefore(root->right, file);
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
        SaveTrieBefore(root->left, file);
    }

    if ( root->index < root->right->index ) {
        SaveTrieBefore(root->right, file);
    }
    
    return true;
}

//----------------------------------------------------------------------------

void TPatriciaTrie::LoadTrieBefore(char* key, TPatriciaTrieItem* root, std::ifstream* file) {
    
    TPack pack;
    
    if( head == root ) {
        file->read((char*)&pack,sizeof(TPack));
        file->read(key, pack.length*sizeof(char));
        key[pack.length] = '\0';
        TPatriciaTrieItem* newNode = new TPatriciaTrieItem();
        if(!newNode) {
            std::cout << "ERROR: bad allocation!" << std::endl;
            exit(0);
        }
        newNode->Initialize(key, pack.data, pack.index, newNode, newNode);
        head->right = newNode;
        return LoadTrieBefore(key, head->right, file);
    }
    else {
        file->read((char*)&pack,sizeof(TPack));
        file->read(key, pack.length*sizeof(char));
        key[pack.length] = '\0';
        if(!pack.up) {
            TPatriciaTrieItem* newNode = new TPatriciaTrieItem();
            if(!newNode) {
                std::cout << "ERROR: bad allocation!" << std::endl;
                exit(0);
            }
            newNode->Initialize(key, pack.data, pack.index, newNode, newNode);
            root->left = newNode;
        }
        else {
            TPatriciaTrieItem* parent  = head;
            TPatriciaTrieItem* current = head->right;
            while(parent->index < current->index) {
                parent = current;
                current = (BitGet(key, current->index)) ? current->right : current->left;
            }
            root->left = current;
        }
        
        file->read((char*)&pack,sizeof(TPack));
        file->read(key, pack.length*sizeof(char));
        key[pack.length] = '\0';
        if(!pack.up) {
            TPatriciaTrieItem* newNode = new TPatriciaTrieItem();
            if(!newNode) {
                std::cout << "ERROR: bad allocation!" << std::endl;
                exit(0);
            }
            newNode->Initialize(key, pack.data, pack.index, newNode, newNode);
            root->right = newNode;
        }
        else {
            TPatriciaTrieItem* parent  = head;
            TPatriciaTrieItem* current = head->right;
            while(parent->index < current->index) {
                parent = current;
                current = (BitGet(key, current->index)) ? current->right : current->left;
            }
            root->right = current;
        }
        
        if ( root->index < root->left->index ) {
            LoadTrieBefore(key, root->left, file);
        }

        if ( root->index < root->right->index ) {
            LoadTrieBefore(key, root->right, file);
        }
        
        return;
    }
}

//----------------------------------------------------------------------------

unsigned long long int* TPatriciaTrie::Lookup(char* k) {

    TPatriciaTrieItem* node = LookupNode(k);

    if (!node) {
        return nullptr;
    }

    return &(node->data);

}

//----------------------------------------------------------------------------

TPatriciaTrieItem* TPatriciaTrie::LookupNode(char* key) {
    
    TPatriciaTrieItem*   prev = head;
    TPatriciaTrieItem*   directNode = head->right;
    
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

bool TPatriciaTrie::Delete(char* key) {
    
    TPatriciaTrieItem *path[2], *directNode, *par, *prev;

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

    if (directNode != path[1]) {
        KeyCopy(path[1], directNode);
    }

    if ((path[1]->left->index > path[1]->index) || (path[1]->right->index > path[1]->index)) {

        if (path[1] != directNode) {

            prev = path[1];
            par  = BitGet(path[1]->key, path[1]->index) ? path[1]->right : path[1]->left;
 
            while (prev->index < par->index) {
                prev = par;
                par  = BitGet(path[1]->key, par->index) ? par->right : par->left;
            }

            if (!KeyCompare(path[1]->key, par->key)) {
                return false;
            }

            if (BitGet(path[1]->key, prev->index)) {
                prev->right = directNode;
            }
            else {
                prev->left  = directNode;
            }
        }

        if (path[0] != path[1]) {
            TPatriciaTrieItem* child = BitGet(key, path[1]->index) ? path[1]->left : path[1]->right;
            if (BitGet(key, path[0]->index)) {
                path[0]->right = child;
            }
            else {
                path[0]->left  = child;
            }
        }
   
    }
    else if (path[0] != path[1]) {
        TPatriciaTrieItem* cLeft = path[1]->left;
        TPatriciaTrieItem* cRight = path[1]->right;
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

void TPatriciaTrie::RecursiveRemove(TPatriciaTrieItem* root) {

    TPatriciaTrieItem* l = root->left;
    TPatriciaTrieItem* r = root->right;

    if ( (l->index >= root->index) && (l != root) && (l != head) )
        RecursiveRemove(l);

    if ( (r->index >= root->index) && (r != root) && (r != head) )
        RecursiveRemove(r);

    delete root;

}

//----------------------------------------------------------------------------

void TPatriciaTrie::ClearTrie() {
    RecursiveRemove(head->right);
    head->right = head;
    return;
}

//----------------------------------------------------------------------------

bool TPatriciaTrie::Empty() {
    if( head->right == head ) {
        return true;
    }
    else {
        return false;
    }
}
//----------------------------------------------------------------------------

int TPatriciaTrie::BitGet(char* key, int n) {
    if (n == -1) {
        return 2;
    }
    int k = (n & 0x7);
    return ( (*(key + (n >> 3))) >> k) & 0x1;
}

//----------------------------------------------------------------------------

bool TPatriciaTrie::KeyCompare(char* key1, char* key2) {
    if (!key1 || !key2) {
        return false;
    }
    return (strcmp((char*)key1, (char*)key2) == 0);
}

//----------------------------------------------------------------------------

int TPatriciaTrie::BitFirstDifferent(char* key1, char* key2) {
    
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

TPatriciaTrieItem* TPatriciaTrie::GetHead() {
    return head;
}

//----------------------------------------------------------------------------

void TPatriciaTrie::SetHead(TPatriciaTrie* nTree) {
    head->right = nTree->head->right;
    return;
}

//----------------------------------------------------------------------------

void TPatriciaTrie::KeyCopy(TPatriciaTrieItem* src, TPatriciaTrieItem* dest) {

    if (src == dest) {
        return;
    }

    if (strlen(dest->key) < strlen(src->key)) {
        dest->key = (char*)realloc(dest->key, 1 + strlen(src->key));
    }
    
    strcpy(dest->key, src->key);

    dest->data = src->data;
    
}
