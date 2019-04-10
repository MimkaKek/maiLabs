#ifndef T_PATRICIA_TRIE_H
#define T_PATRICIA_TRIE_H

#include <cstring>
#include <cstdlib>
//#include <dmalloc.h>

class TPatriciaTrie;

class TPatriciaTrieItem {
    private:
        
        friend class TPatriciaTrie;
        
        unsigned long long int  data;
        char*                   key;
        int                     index;
        TPatriciaTrieItem*      left;
        TPatriciaTrieItem*      right;

    public:

        TPatriciaTrieItem();
        TPatriciaTrieItem(char*, unsigned long long int, int, TPatriciaTrieItem*, TPatriciaTrieItem*);
        virtual ~TPatriciaTrieItem();

        void                    Initialize(char*, unsigned long long int, int, TPatriciaTrieItem*, TPatriciaTrieItem*);
};

#endif
