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

template <class T>
class TPatriciaTrie {
    private:
        
        void                    RecursiveRemove(TPatriciaTrieItem<T>*);
        int                     BitGet(char*, int);
        int                     BitFirstDifferent(char*, char*);
        bool                    KeyCompare(char*, char*);
        void                    KeyCopy(TPatriciaTrieItem<T>*, TPatriciaTrieItem<T>*);
        
        TPatriciaTrieItem<T>*   head;

    public:

        TPatriciaTrie();
        virtual ~TPatriciaTrie();
        
        TPatriciaTrieItem<T>*   LookupNode(char*);
        T*                      Lookup(char*);
        TPatriciaTrieItem<T>*   Insert(char*, T);
        bool                    Delete(char*);
        
        TPatriciaTrieItem<T>*   GetHead();
        void                    Print(TPatriciaTrieItem<T>*, int);
        void                    ClearTrie();
        bool                    Empty();
        void                    SwapHead(TPatriciaTrie<T>*);
        
        bool                    SaveTrieBefore(TPatriciaTrieItem<T>*, std::ofstream*);
        bool                    SaveTrieCurrent(TPatriciaTrieItem<T>*, std::ofstream*);
        
        void                    LoadTrieBefore(char*, std::ifstream*);
        void                    LoadTrieCurrent(char*, TPatriciaTrieItem<T>*, std::ifstream*);
};

#endif
