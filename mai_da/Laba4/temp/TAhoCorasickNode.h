#ifndef T_AHO_CORASICK_NODE_H
#define T_AHO_CORASICK_NODE_H

#include <map>

class TAhoCorasick;
class TAKNode;

typedef std::map<const char, TAKNode*> LinksMap;

class TAKNode {
private:

    friend class TAhoCorasick;

    LinksMap        links;
    TAKNode*        fail;
    TAKNode*        term;
    int             out;

public:
    
    TAKNode();
    TAKNode(TAKNode*);
    
    TAKNode*        GetLink(char) const;
    bool            IsTerminal() const;
};

#endif
