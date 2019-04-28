#include <iostream>
#include <queue>
#include "TAhoCorasick.h"
#include "Settings.h"


TAhoCorasick::TAhoCorasick() {
    state = nullptr;
    lastStrNumb = 0;
    size = 0;
}

TAhoCorasick::~TAhoCorasick() {
    ClearTree();
}

void TAhoCorasick::AddToPattern(unsigned long long int numb) {
    TAKNode* child = state->GetLink(numb);
    if(!child) {
        child = new TAKNode(&root);
        child->deep = state->deep + 1;
        state->links[numb] = child;
    }
    state = child;
}

void TAhoCorasick::FinPattern() {
    ++size;
    state->PushNumber(size);
    state = &root;
}

void TAhoCorasick::ToStart() {
    state = &root;
    return;
}

void TAhoCorasick::Build() {
    
    std::queue<TAKNode*> nodes;
    nodes.push(&root);
    
    while(!nodes.empty()) {
        
        TAKNode* current = nodes.front();
        nodes.pop();
        
        for(TLinksMap::const_iterator iter = current->links.cbegin(); iter != current->links.cend(); ++iter) {
            
            unsigned long long int      numb   = iter->first;
            TAKNode*                    child  = iter->second;
            TAKNode*                    tmp    = current->fail;
            
            while(tmp) {
                TAKNode* candidate = tmp->GetLink(numb);
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


void TAhoCorasick::Step(unsigned long long int numb) {
    
    while(state) {
        TAKNode* candidate = state->GetLink(numb);
        if (candidate) {
            state = candidate;
            return;
        }
        state = state->fail;
    }
    
    state = &root;
}

void TAhoCorasick::EmptyStr() {
    tableOfStr.push_back(0);
    lastStrNumb = tableOfStr.size();
    if(DEBUG_GET_EMPTY_STR) {
        std::cout << "Empty string! Number = " << lastStrNumb <<std::endl;
    }
}

void TAhoCorasick::PrintTermsForCurrentState() const {
    
    unsigned int    strNumb;
    int             posNumb;
    
    if (state->IsTerminal()) {
        if(DEBUG_PARAM_FUN) {
            std::cout << "Current position in string = " << tableOfStr[lastStrNumb - 1] << std::endl;
            std::cout << "Number of string = " << lastStrNumb << std::endl;
            std::cout << "Count of calls = " << state->GetCount() << std::endl;
            std::cout << "Result:" << std::endl;
        }
        posNumb = tableOfStr[lastStrNumb - 1] - state->deep + 1;
        strNumb = lastStrNumb;
        while(posNumb <= 0) {
            --strNumb;
            posNumb = tableOfStr[strNumb - 1] + posNumb;
        }
        if(!BENCHMARK) {
            for(int i = 0; i < state->GetCount(); ++i) {
                std::cout << strNumb << ", " << posNumb << ", " << state->numb[i] << std::endl;
            }
        }
    }
    
    TAKNode* tmp = state->term;
    while (tmp) {
        if(DEBUG_PARAM_FUN) {
            std::cout << "Current position in string = " << tableOfStr[lastStrNumb - 1] - tmp->deep + 1 << std::endl;
            std::cout << "Number of string = " << lastStrNumb << std::endl;
            std::cout << "Count of calls = " << tmp->GetCount() << std::endl;
            std::cout << "Result:" << std::endl;
        }
        posNumb = tableOfStr[lastStrNumb - 1] - tmp->deep + 1;
        strNumb = lastStrNumb;
        while(posNumb <= 0) {
            --strNumb;
            posNumb = tableOfStr[strNumb - 1] + posNumb;
        }
        if(!BENCHMARK) {
            for(int i = 0; i < tmp->GetCount(); ++i) {
                std::cout << strNumb << ", " << posNumb << ", " << tmp->numb[i] << std::endl;
            }
        }
        tmp = tmp->term;
    }
}

void TAhoCorasick::ClearTree() {
    std::queue<TAKNode*> nodes;
    nodes.push(&root);
    
    while(!nodes.empty()) {
        
        TAKNode* current = nodes.front();
        nodes.pop();
        
        for(TLinksMap::const_iterator iter = current->links.cbegin(); iter != current->links.cend(); ++iter) {
            nodes.push(iter->second);
        }
        
        if(current != &root) {
            delete current;
        }
    }
    
    state = &root;
    size = 0;
    lastStrNumb = 0;
    tableOfStr.clear();
    
    return;
}

void TAhoCorasick::Search(unsigned long long int numb, unsigned int strNumb) {
    if(DEBUG_SEARCH) {
        std::cout << "Search: " << std::endl;
        std::cout << "numb = " << numb << " | strNumb = " << strNumb << std::endl;
    }
    Step(numb);
    if(strNumb > lastStrNumb) {
        tableOfStr.push_back(1);
        lastStrNumb = tableOfStr.size();
        if(DEBUG_GET_NEW_STR) {
            std::cout << "New string get! Number = " << lastStrNumb << std::endl;
        }
    }
    else {
        ++tableOfStr[lastStrNumb - 1];
    }
    PrintTermsForCurrentState();
}


