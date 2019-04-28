#ifndef T_AHO_CORASICK_H
#define T_AHO_CORASICK_H

#include <string>
#include <vector>
#include <stack>
#include "TAhoCorasickNode.h"

class TAhoCorasick {
private:
    TAKNode                                             root;
    TAKNode*                                            state;
    unsigned int                                        size;
    unsigned int                                        lastStrNumb;
    std::vector<int>                                    tableOfStr;
    
    void                                                Step(unsigned long long int);
    void                                                PrintTermsForCurrentState() const;
public:

    TAhoCorasick();
    ~TAhoCorasick();
    
    void                                                ClearTree();
    void                                                EmptyStr();
    void                                                ToStart();
    void                                                FinPattern();
    void                                                AddToPattern(unsigned long long int);
    void                                                Build();
    void                                                Search(unsigned long long int, unsigned int);
};

#endif
