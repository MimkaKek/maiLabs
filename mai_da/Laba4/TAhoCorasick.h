#ifndef T_AHO_CORASICK_H
#define T_AHO_CORASICK_H

#include <string>
#include <vector>
#include "TAhoCorasickNode.h"

class TAhoCorasick {
private:
    TAKNode                     root;
    std::vector<std::string>    words;
    TAKNode*                    state;
    
    void                        Step(char);
    void                        PrintTermsForCurrentState() const;
public:

    TAhoCorasick();

    void                        AddString(const char* const);
    void                        Initialize();
    void                        Search(const char* const);
};

#endif
