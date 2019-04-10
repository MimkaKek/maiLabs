#ifndef T_PATRICIA_TRIE_ITEM_H
#define T_PATRICIA_TRIE_ITEM_H

#include "TPatriciaTrieItem.h"
#include <fstream>

typedef struct {
    char                    up;
    int                     length;
    unsigned long long int  data;
    int                     index;
} TPack;

class TPatriciaTrie {
    private:
        
        void                    RecursiveRemove(TPatriciaTrieItem*);
        int                     BitGet(char*, int);
        int                     BitFirstDifferent(char*, char*);
        bool                    KeyCompare(char*, char*);
        void                    KeyCopy(TPatriciaTrieItem*, TPatriciaTrieItem*);
        
        char                    mask[256];
        TPatriciaTrieItem*      head;

    public:

        TPatriciaTrie();
        virtual ~TPatriciaTrie();
        
        TPatriciaTrieItem*      LookupNode(char*);
        unsigned long long int* Lookup(char*);
        TPatriciaTrieItem*      Insert(char*, unsigned long long int);
        bool                    Delete(char*);
        
        TPatriciaTrieItem*      GetHead();
        void                    Print(TPatriciaTrieItem*, int);
        void                    ClearTrie();
        bool                    Empty();
        void                    SetHead(TPatriciaTrie*);
        
        bool                    SaveTrieCurrent(TPatriciaTrieItem*, std::ofstream*);
        bool                    SaveTrieBefore(TPatriciaTrieItem*, std::ofstream*);
        
        void                    LoadTrieCurrent(char*, std::ifstream*);
        void                    LoadTrieBefore(char*, TPatriciaTrieItem*, std::ifstream*);
};

#endif
