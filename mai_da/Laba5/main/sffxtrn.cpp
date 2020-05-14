#include "sffxtrn.h"

TNode::TNode(size_t l, size_t *r, size_t n, TNode *node) : left(l), right(r), number(n), suffLink(node) { }

TNode*& TNode::getEdge(const std::string &str, size_t edge) {
    return edges[str[edge]];
}

size_t TNode::getLeft() {
    return left;
}

size_t TNode::getRight() {
    return number == INTERNAL ? (size_t)right : *right;
}

size_t TNode::getLength() {
    return getRight() - getLeft() + 1;
}

void TNode::genVector() {

    for(auto it = edges.begin(); it != edges.end(); ++it) {

        sEdges.push_back(&(*it));

        for(size_t i = sEdges.size() - 1; i > 0; --i) {
            if(sEdges[i]->first < sEdges[i - 1]->first) {
                std::pair<const char, TNode*>* tmp = sEdges[i - 1];
                sEdges[i - 1] = sEdges[i];
                sEdges[i] = tmp;
            }
            else {
                break;
            }
        }
        
    }

    return;
}

void TNode::Print(const std::string &str) {
    size_t rightBound = getRight();
    for (size_t i = left; i <= rightBound; i++) {
        std::cout << str[i];
    }
    if (number != INTERNAL) {
        std::cout << " - " << number;
    }
}
