#ifndef T_SFFX_TR_H
#define T_SFFX_TR_H

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <utility>

#include "Globals.h"
#include "sffxtrn.h"

class TSuffTree {
    public:
        TSuffTree(const std::string&);
        
        ~TSuffTree();
        
        void                Phase(size_t);
        
        void                Print();
        
        void                Task();
        
    private:
    
        std::string         str;
        char*               answer;
        size_t              ansPos;
        TNode*              root;
        
        TNode*              activeNode;
        size_t              activeEdge;
        size_t              activeLength;
        
        size_t              end;
        size_t              remain;
        size_t              suffNum;
        
        bool              GetLowest(size_t, TNode*);
        TNode*              Insert(TNode*);
        void                print(TNode*, int);
        void                print_suff_links(TNode*);
        int                 Walkdown();
        char                getNextCharAct();
        void                DeleteTree(TNode*);
};

#endif
