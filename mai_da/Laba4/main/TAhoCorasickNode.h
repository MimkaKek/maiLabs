#ifndef T_AHO_CORASICK_NODE_H
#define T_AHO_CORASICK_NODE_H

#include <map>
#include <vector>

class TAhoCorasick;
class TAKNode;

typedef std::map<unsigned long long int, TAKNode*> TLinksMap;

class TAKNode {
private:

    friend class TAhoCorasick;

    TLinksMap           links;
    TAKNode*            fail;
    TAKNode*            term;
    std::vector<int>    numb;
    int                 deep;

public:
    
    TAKNode();
    TAKNode(TAKNode*);
    
    void            PushNumber(int);
    int             GetCount() const;
    TAKNode*        GetLink(unsigned long long int) const;
    bool            IsTerminal() const;
};

#endif
