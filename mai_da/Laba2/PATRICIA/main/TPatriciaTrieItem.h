#ifndef T_PATRICIA_TRIE_H
#define T_PATRICIA_TRIE_H

template <class T> class TPatriciaTrie;

template <class T>
class TPatriciaTrieItem {
    private:
        
        friend class TPatriciaTrie<T>;
        
        T                       data;
        char*                   key;
        int                     index;
        TPatriciaTrieItem<T>*   left;
        TPatriciaTrieItem<T>*   right;

    public:

        TPatriciaTrieItem();
        TPatriciaTrieItem(char*, T, int, TPatriciaTrieItem<T>*, TPatriciaTrieItem<T>*);
        virtual ~TPatriciaTrieItem();

        void                    Initialize(char*, T, int, TPatriciaTrieItem<T>*, TPatriciaTrieItem<T>*);

        T                       GetData();
        bool                    SetData(T);
        
        char*                   GetKey();

        TPatriciaTrieItem<T>*   GetLeft();
        TPatriciaTrieItem<T>*   GetRight();

};

#endif
