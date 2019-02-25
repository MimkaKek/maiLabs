//----------------------------------------------------------------------------
//
// PATRICIA Trie Template Class
//
// Released into the public domain on February 3, 2005 by:
//
//      Radu Gruian
//      web:   http://www.gruian.com
//      email: gruian@research.rutgers.edu
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to:
//
//      Free Software Foundation, Inc.
//      59 Temple Place, Suite 330
//      Boston, MA 02111-1307
//      USA
//
//----------------------------------------------------------------------------
//
// File:    PatriciaTrie.h
// Date:    02/03/2005
// Purpose: Patricia trie ADT.
//
//----------------------------------------------------------------------------
 
#ifndef PatriciaTrieH
#define PatriciaTrieH
 
#include <stdlib.h>
#include <string.h>

const unsigned int MAX_DEEP = 10;

template <class T> class PatriciaTrie;
 
 
//----------------------------------------------------------------------------
//
// Class:   PatriciaTrieNode
// Purpose: A node in a PATRICIA trie.
//          Each node stores one key, and the data associated with that key.
//
//----------------------------------------------------------------------------
template <class T>
class PatriciaTrieNode {
        private:
                friend class PatriciaTrie<T>;
                int bit_index;
                char*        key;
                T                       data;
                PatriciaTrieNode<T>*   left;
                PatriciaTrieNode<T>*   right;
 
        public:
 
                // Constructors & destructor
                PatriciaTrieNode();
                PatriciaTrieNode(char*, T, int, PatriciaTrieNode<T>*, PatriciaTrieNode<T>*);
                virtual ~PatriciaTrieNode();
 
                // Name:    Initialize
                // Args:    key, data, left, right
                // Return:  void
                // Purpose: Initialize this node with the given data.
                void                                    Initialize(char*, T, int, PatriciaTrieNode<T>*, PatriciaTrieNode<T>*);
 
                // Name:        GetData/SetData
                // Args:        data : T
                // Return:      T | bool
                // Purpose:     Accessors for the data field.
                T                       GetData();
                bool                    SetData(T);
               
                // Name:        GetKey
                // Args:        none
                // Return:      char*
                // Purpose:     Getter for the key field.
                char*        GetKey();
 
                // Name:        GetLeft/GetRight
                // Args:        none
                // Return:      PatriciaTrieNode*
                // Purpose:     Getters for the left/right fields.
                PatriciaTrieNode<T>*   GetLeft();
                PatriciaTrieNode<T>*   GetRight();
 
};
 
//----------------------------------------------------------------------------
//
// Class:   PatriciaTrie
// Purpose: Implements a PATRICIA trie structure with keys of
//          type char* (currently char*, but can be changed, see
//          the definition of char* above).
//
//----------------------------------------------------------------------------
template <class T>
class PatriciaTrie {
        private:
                void recursive_remove(PatriciaTrieNode<T>*);
                int  bit_get(char*, int);
                int  bit_first_different(char*, char*);
                bool key_compare(char*, char*);
                void key_copy(PatriciaTrieNode<T>*, PatriciaTrieNode<T>*);
                PatriciaTrieNode<T>* head;
 
        public:
 
                // Constructor and destructor
                PatriciaTrie();
                virtual ~PatriciaTrie();
 
                // Name:        Insert(key, data)
                // Args:        key : char*, data : T
                // Return:      PatriciaTrieNode*
                // Purpose:     Insert a new key+data pair in the Patricia structure, and
                //          return the new node.
                virtual PatriciaTrieNode<T>*   Insert(char*, T);
 
                // Name:        Lookup(key)
                // Args:        key : char*
                // Return:      T
                // Purpose:     Search for the given key, and return the data associated
                //          with it (or NULL).
                virtual T                       Lookup(char*);
                virtual void                    Print(PatriciaTrieNode<T>*, int);
                virtual bool                    SaveTrie(PatriciaTrieNode<T>* , std::ofstream*);
                virtual void                    LoadTrie(char*, PatriciaTrieNode<T>*, std::ifstream*);
                virtual PatriciaTrieNode<T>*    GetHead();
 
                // Name:        LookupNode(key)
                // Args:        key : char*
                // Return:      T
                // Purpose:     Search for the given key, and return the node that
                //          contains it (or NULL).
                virtual PatriciaTrieNode<T>*   LookupNode(char*);
 
                // Name:        Delete(key)
                // Args:        key : char*
                // Return:      bool
                // Purpose:     Remove the node containing the given key. Return
        //          true if the operation succeeded, false otherwise.
                virtual bool                    Delete(char*);
 
};
 
//----------------------------------------------------------------------------
template <class T>
PatriciaTrieNode<T>::PatriciaTrieNode() {
        Initialize(NULL, 0, -1, this, this);
}
 
//----------------------------------------------------------------------------
template <class T>
PatriciaTrieNode<T>::PatriciaTrieNode(char* k,
                                        T d,
                                        int bi,
                                        PatriciaTrieNode<T>* l,
                                        PatriciaTrieNode<T>* r) {
    Initialize(k, d, bi, l, r);
}
 
//----------------------------------------------------------------------------
template <class T>
void PatriciaTrieNode<T>::Initialize(char* k,
                                      T d,
                                      int bi,
                                      PatriciaTrieNode<T>* l,
                                      PatriciaTrieNode<T>* r) {
        if (k)
                key = (char*)strdup(k);
        else
                key = k;
        data      = d;
        left      = l;
        right     = r;
        bit_index = bi;
}
 
//----------------------------------------------------------------------------
template <class T>
PatriciaTrieNode<T>::~PatriciaTrieNode() {
        if (key) {
                free(key);
                key = NULL;
        }
}
 
//----------------------------------------------------------------------------
template <class T>
T PatriciaTrieNode<T>::GetData() {
        return data;
}
 
//----------------------------------------------------------------------------
template <class T>
bool PatriciaTrieNode<T>::SetData(T d) {
        memcpy(&data, &d, sizeof(T));
        return true;
}
 
//----------------------------------------------------------------------------
template <class T>
char* PatriciaTrieNode<T>::GetKey() {
        return key;
}
 
//----------------------------------------------------------------------------
template <class T>
PatriciaTrieNode<T>* PatriciaTrieNode<T>::GetLeft() {
        return left;
}
 
//----------------------------------------------------------------------------
template <class T>
PatriciaTrieNode<T>* PatriciaTrieNode<T>::GetRight() {
        return right;
}
 
//----------------------------------------------------------------------------
template <class T>
PatriciaTrie<T>::PatriciaTrie() {
        // Create the head of the structure. The head is never moved
        // around in the trie (i.e. it always stays at the top of the structure).
    // This prevents further complications having to do with node removal.
        head = new PatriciaTrieNode<T>();
#define ZEROTAB_SIZE 256
        head->key = (char*)calloc(ZEROTAB_SIZE, 1);
}
 
//----------------------------------------------------------------------------
template <class T>
PatriciaTrie<T>::~PatriciaTrie() {
        recursive_remove(head);
}
 
//----------------------------------------------------------------------------
template <class T>
PatriciaTrieNode<T>* PatriciaTrie<T>::Insert(char* k, T d) {
       
        PatriciaTrieNode<T> *p, *t, *x;
 
        // Start at the root
        p = head;
        t = (PatriciaTrieNode<T>*)(p->right);
 
        // Navigate down the tree and look for the key
        while (p->bit_index < t->bit_index) {
                p = t;
                t = (PatriciaTrieNode<T>*)(bit_get(k, t->bit_index) ? t->right : t->left);
        }
 
        // Is the key already in the tree?
        if (key_compare(k, t->key))
                return NULL; // Already in the tree!
 
        // Find the first bit that does not match.
        int i = bit_first_different(k, t->key);
 
        // Find the appropriate place in the tree where
        // the node has to be inserted
        p  = head;
        x  = (PatriciaTrieNode<T>*)(p->right);
        while ( ( p->bit_index < x->bit_index ) &&
                        ( x->bit_index < i) ) {
                p = x;
                x = (PatriciaTrieNode<T>*)(bit_get(k, x->bit_index) ? x->right : x->left);
        }
 
        // Allocate a new node and initialize it.
        t = new PatriciaTrieNode<T>();
        t->Initialize(k, d, i, (bit_get(k, i) ? x : t), (bit_get(k, i) ? t : x));
 
        // Rewire
        if (bit_get(k, p->bit_index))
                p->right = t;
        else
                p->left = t;
 
        // Return the newly created node
        return t;
 
}
 
//----------------------------------------------------------------------------
template <class T>
T PatriciaTrie<T>::Lookup(char* k) {
 
        // Lookup the node
        PatriciaTrieNode<T>* node = LookupNode(k);
 
        // Failed?
        if (!node) {
            return 0;
        }
        // Return the data stored in this node
        return node->data;
 
}

//----------------------------------------------------------------------------
template <class T>
PatriciaTrieNode<T>* PatriciaTrie<T>::GetHead() {
    return head;
}

//----------------------------------------------------------------------------
template <class T>
void PatriciaTrie<T>::Print(PatriciaTrieNode<T>* root, int deep) {
    
    int pDeep;
    
    if ( root->bit_index < root->left->bit_index ) {
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
    std::cout << root->data << ':' << root->bit_index;
    pDeep = deep;
    while(pDeep < MAX_DEEP) {
        ++pDeep;
        std::cout << "\t|";
    }
    std::cout << std::endl;

    if ( root->bit_index < root->right->bit_index ) {
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
bool PatriciaTrie<T>::SaveTrie(PatriciaTrieNode<T>* root, std::ofstream* file) {
    
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
        file->write((char*)&root->right->bit_index, sizeof(int));
        return SaveTrie(root->right, file);
    }
    
    up = ( root->bit_index < root->left->bit_index || ( root->left->bit_index == 0 && root->left != head->right && root->left != root ) ) ? 0 : 1;
    file->write((char*)&up, sizeof(char));
    length = strlen(root->left->key);
    file->write((char*)&length, sizeof(int));
    file->write(root->left->key, sizeof(char)*length);
    if(!up) {
        file->write((char*)&root->left->data, sizeof(T));
        file->write((char*)&root->left->bit_index, sizeof(int));
    }
    
    up = ( root->bit_index < root->right->bit_index || ( root->right->bit_index == 0 && root->right != head->right && root->right != root ) ) ? 0 : 1;
    file->write((char*)&up, sizeof(char));
    length = strlen(root->right->key);
    file->write((char*)&length, sizeof(int));
    file->write(root->right->key, sizeof(char)*length);
    if(!up) {
        file->write((char*)&root->right->data, sizeof(T));
        file->write((char*)&root->right->bit_index, sizeof(int));
    }
    
    if ( root->bit_index < root->left->bit_index || ( root->left->bit_index == 0 && root->left != head->right && root->left != root ) ) {
        if(!SaveTrie(root->left, file)) {
            return false;
        }
    }

    if ( root->bit_index < root->right->bit_index || ( root->right->bit_index == 0 && root->right != head->right && root->right != root ) ) {
        if(!SaveTrie(root->right, file)) {
            return false;
        }
    }
    
    return true;
}

//----------------------------------------------------------------------------
template <class T>
void PatriciaTrie<T>::LoadTrie(char* key, PatriciaTrieNode<T>* root, std::ifstream* file) {
    
    int length = 0;
    int bit_index = 0;
    T data = 0;
    char up = 0;
    
    if( head == root ) {
        file->read((char*)&up,sizeof(char));
        file->read((char*)&length,sizeof(int));
        file->read(key, length*sizeof(char));
        key[length] = '\0';
        file->read((char*)&data,sizeof(T));
        file->read((char*)&bit_index,sizeof(int));
        PatriciaTrieNode<T>* newNode = new PatriciaTrieNode<T>();
        if(!newNode) {
            std::cout << "ERROR: bad allocation!" << std::endl;
            exit(0);
        }
        newNode->Initialize(key, data, bit_index, newNode, newNode);
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
            file->read((char*)&bit_index,sizeof(int));
            PatriciaTrieNode<T>* newNode = new PatriciaTrieNode<T>();
            if(!newNode) {
                std::cout << "ERROR: bad allocation!" << std::endl;
                exit(0);
            }
            newNode->Initialize(key, data, bit_index, newNode, newNode);
            root->left = newNode;
        }
        else {
            PatriciaTrieNode<T>* tmp = head->right;
            while(tmp != root && strcmp(tmp->key, key) != 0) {
                tmp = (bit_get(root->key, tmp->bit_index)) ? tmp->right : tmp->left;
            }
            root->left = tmp;
        }
        file->read((char*)&up,sizeof(char));
        file->read((char*)&length,sizeof(int));
        file->read(key, length*sizeof(char));
        key[length] = '\0';
        if(!up) {
            file->read((char*)&data,sizeof(T));
            file->read((char*)&bit_index,sizeof(int));
            PatriciaTrieNode<T>* newNode = new PatriciaTrieNode<T>();
            if(!newNode) {
                std::cout << "ERROR: bad allocation!" << std::endl;
                exit(0);
            }
            newNode->Initialize(key, data, bit_index, newNode, newNode);
            root->right = newNode;
        }
        else {
            PatriciaTrieNode<T>* tmp = head->right;
            while(tmp != root && strcmp(tmp->key, key) != 0) {
                tmp = (bit_get(root->key, tmp->bit_index)) ? tmp->right : tmp->left;
            }
            root->right = tmp;
        }
        
        if ( root->bit_index < root->left->bit_index ) {
            LoadTrie(key, root->left, file);
        }

        if ( root->bit_index < root->right->bit_index ) {
            LoadTrie(key, root->right, file);
        }
        
        return;
    }
}
//----------------------------------------------------------------------------
template <class T>
PatriciaTrieNode<T>* PatriciaTrie<T>::LookupNode(char* k) {
 
        PatriciaTrieNode<T>* p;
        PatriciaTrieNode<T>* x;
 
        // Start at the root.
    p = head;
        x = (PatriciaTrieNode<T>*)(head->right);
 
        // Go down the Patricia structure until an upward
        // link is encountered.
        while (p->bit_index < x->bit_index) {
                p = x;
                x = (PatriciaTrieNode<T>*)(bit_get(k, x->bit_index) ? x->right : x->left);
        }
 
        // Perform a full string comparison, and return NULL if
        // the key is not found at this location in the structure.
        if (!key_compare(k, x->key))
                return NULL;
 
        // Return the node
        return x;
 
}
 
//----------------------------------------------------------------------------
template <class T>
bool PatriciaTrie<T>::Delete(char* k) {
 
        PatriciaTrieNode<T> *p, *t, *x, *pp, *lp;
        int bp, bl, br;
        char* key = NULL;
 
        // Start at the root
        p  = head;
        t  = (PatriciaTrieNode<T>*)(p->right);
 
        // Navigate down the tree and look for the key
        while (p->bit_index < t->bit_index) {
                pp = p;
                p  = t;
                t  = (PatriciaTrieNode<T>*)(bit_get(k, t->bit_index) ? t->right : t->left);
        }
 
        // Is the key in the tree? If not, get out!
        if (!key_compare(k, t->key))
                return false; // The key could not be found!
 
        // Copy p's key to t
        if (t != p)
                key_copy(p, t);
 
        // Is p a leaf?
        bp = p->bit_index;
        bl = ((PatriciaTrieNode<T>*)(p->left))->bit_index;
        br = ((PatriciaTrieNode<T>*)(p->right))->bit_index;
 
        if ((bl > bp) || (br > bp)) {
               
        // There is at least one downward edge.
 
                if (p != t) {
                       
                        // Look for a new (intermediate) key
                        key = strdup(p->key);
 
                        lp = p;
                        x  = (PatriciaTrieNode<T>*)(bit_get(key, p->bit_index) ? p->right : p->left);
     
                        while (lp->bit_index < x->bit_index) {
                                lp = x;
                                x  = (PatriciaTrieNode<T>*)(bit_get(key, x->bit_index) ? x->right : x->left);
                        }
 
                        // If the intermediate key was not found, we have a problem..
            if (!key_compare(key, x->key)) {
                free(key);
                                return false; // The key could not be found!
            }
 
                        // Rewire the leaf (lp) to point to t
                        if (bit_get(key, lp->bit_index))
                                lp->right = t;
                        else
                                lp->left = t;
 
                }
 
                // Rewire the parent to point to the real child of p
                if (pp != p) {
                        PatriciaTrieNode<T>* ch = (PatriciaTrieNode<T>*)(bit_get(k, p->bit_index) ? p->left : p->right);
                        if (bit_get(k, pp->bit_index))
                                pp->right = ch;
                        else
                                pp->left = ch;
                }
 
        // We no longer need 'key'
        free(key);
        key = NULL;
       
        } else {
 
                // Both edges (left, right) are pointing upwards or to the node (self-edges).
   
                // Rewire the parent
                if (pp != p) {
                        PatriciaTrieNode<T>* blx = (PatriciaTrieNode<T>*)(p->left);
                        PatriciaTrieNode<T>* brx = (PatriciaTrieNode<T>*)(p->right);
                        if (bit_get(k, pp->bit_index))
                                pp->right = (((blx == brx) && (blx == p)) ? pp : ((blx==p)?brx:blx));
                        else
                                pp->left  = (((blx == brx) && (blx == p)) ? pp : ((blx==p)?brx:blx));
                }
 
        }
 
        // Deallocate p (no longer needed)
        delete p;
 
        // Success!
        return true;
 
}
 
//----------------------------------------------------------------------------
template <class T>
void PatriciaTrie<T>::recursive_remove(PatriciaTrieNode<T>* root) {
 
        PatriciaTrieNode<T>* l = (PatriciaTrieNode<T>*)root->left;
        PatriciaTrieNode<T>* r = (PatriciaTrieNode<T>*)root->right;
 
        // Remove the left branch
        if ( (l->bit_index >= root->bit_index) && (l != root) && (l != head) )
                recursive_remove(l);
 
        // Remove the right branch
        if ( (r->bit_index >= root->bit_index) && (r != root) && (r != head) )
                recursive_remove(r);
 
        // Remove the root
        delete root;
 
}
 
//----------------------------------------------------------------------------
template <class T>
int PatriciaTrie<T>::bit_get(char* bit_stream, int n) {
  if (n < 0) return 2; // "pseudo-bit" with a value of 2.
  int k = (n & 0x7);
  return ( (*(bit_stream + (n >> 3))) >> k) & 0x1;
}
 
//----------------------------------------------------------------------------
template <class T>
bool PatriciaTrie<T>::key_compare(char* k1, char* k2) {
    if (!k1 || !k2) {
        return false;
    }
    return (strcmp((char*)k1, (char*)k2) == 0);
}
 
//----------------------------------------------------------------------------
template <class T>
int PatriciaTrie<T>::bit_first_different(char* k1, char* k2) {
    if (!k1 || !k2) {
        return 0;
    }   // First bit is different!
    int n = 0;
    int d = 0;
    while ( (k1[n] == k2[n]) &&
                    (k1[n] != 0) &&
                    (k2[n] != 0) )
            n++;
    while (bit_get(&k1[n], d) == bit_get(&k2[n], d))
            d++;
    return ((n << 3) + d);
}
 
//----------------------------------------------------------------------------
template <class T>
void PatriciaTrie<T>::key_copy(PatriciaTrieNode<T>* src, PatriciaTrieNode<T>* dest) {
 
        if (src == dest)
            return;
 
        // Copy the key from src to dest
        if (strlen(dest->key) < strlen(src->key))
            dest->key = (char*)realloc(dest->key, 1 + strlen(src->key));
        strcpy(dest->key, src->key);
 
        // Copy the data from src to dest
        dest->data = src->data;
 
        // How about the bit index?
        //dest->bit_index = src->bit_index;
       
}
 
 
#endif
