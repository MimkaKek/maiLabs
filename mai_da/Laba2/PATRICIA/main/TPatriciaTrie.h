#ifndef T_PATRICIA_TRIE_ITEM_H
#define T_PATRICIA_TRIE_ITEM_H

#include "TPatriciaTrieItem.h"
#include <fstream>

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

        TPatriciaTrieItem<T>*   Insert(char*, T);
        TPatriciaTrieItem<T>*   GetHead();
        TPatriciaTrieItem<T>*   LookupNode(char*);
        T*                      Lookup(char*);
        bool                    Delete(char*);
        void                    Print(TPatriciaTrieItem<T>*, int);
        void                    ClearTrie();
        bool                    Empty();
        bool                    SaveTrie_Current(TPatriciaTrieItem<T>*, std::ofstream*);
        bool                    SaveTrie_Before(TPatriciaTrieItem<T>*, std::ofstream*);
        void                    LoadTrie_Current(char*, std::ifstream*);
        void                    LoadTrie_Before(char*, TPatriciaTrieItem<T>*, std::ifstream*);
};

#endif
