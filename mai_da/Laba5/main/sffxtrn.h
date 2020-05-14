#ifndef T_SFFX_TR_N_H
#define T_SFFX_TR_N_H

#include <map>
#include <iostream>
#include <vector>
#include "Globals.h"

class TNode {
    public:
        std::map<const char, TNode*>                edges;
        std::vector<std::pair<const char, TNode*>*> sEdges;
        size_t                                      left;
        size_t*                                     right;
        size_t                                      number;
        TNode*                                      suffLink;

        TNode(size_t, size_t*, size_t, TNode*);

        TNode*&                                     getEdge(const std::string&, size_t);
        size_t                                      getLeft();
        size_t                                      getRight();
        size_t                                      getLength();
        void                                        genVector();
        void                                        Print(const std::string&);
};

#endif
