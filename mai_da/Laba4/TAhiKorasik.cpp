#include <iostream>
#include <queue>
#include "TAhiKorasik.h"


TAhoCorasick::TAhoCorasick() {
    state = nullptr;
}   

void TAhoCorasick::AddString(const char* const str) {
    TAKNode* current = &root;
    for(const char* ch = str; *ch; ++ch) {
        TAKNode* child = current->GetLink(*ch);
        if(!child) {
            child = new TAKNode(&root);
            current->links[*ch] = child;
        }
        current = child;
    }
    current->out = words.size();
    words.push_back(str);
}

void TAhoCorasick::Initialize() {
    
    std::queue<TAKNode*> nodes;
    nodes.push(&root);
    
    while(!nodes.empty()) {
        
        TAKNode* current = nodes.front();
        nodes.pop();
        
        for(LinksMap::const_iterator iter = current->links.cbegin(); iter != current->links.cend(); ++iter) {
            
            char symbol = iter->first;
            TAKNode* child = iter->second;
            TAKNode* tmp = child->fail;
            
            while(tmp) {
                TAKNode* candidate = tmp->GetLink(symbol);
                if(candidate) {
                    child->fail = candidate;
                    break;
                }
                tmp = tmp->fail;
            }
            
            if (child->fail->IsTerminal()) {
                child->term = child->fail;
            }
            else {
                child->term = child->fail->term;
            }
            
            nodes.push(child);
        }
    }
    
    return;
}


void TAhoCorasick::Step(char ch) {
    
    while(state) {
        TAKNode* candidate = state->GetLink(ch);
        if (candidate) {
            state = candidate;
            return;
        }
        state = state->fail;
    }
    
    state = &root;
}

void TAhoCorasick::PrintTermsForCurrentState() const {
    
    if (state->IsTerminal()) {
        std::cout << "found substring " << words[state->out].c_str() << std::endl;
    }
    TAKNode* tmp = state->term;
    while (tmp) {
        std::cout << "found substring " << words[tmp->out].c_str() << std::endl; 
        tmp = tmp->term;
    }
}

void TAhoCorasick::Search(const char* const str) {
    state = &root;
    for(const char* ch = str; *ch; ++ch) {
        std::cout << *ch << ':' << std::endl;
        Step(*ch);
        PrintTermsForCurrentState();
     }
}


