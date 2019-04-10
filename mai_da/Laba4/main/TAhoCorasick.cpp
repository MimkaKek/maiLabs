#include <iostream>
#include <queue>
#include "TAhoCorasick.h"
#include "Settings.h"


TAhoCorasick::TAhoCorasick() {
    state = nullptr;
    lastStrNumb = 0;
    size = 0;
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

void TAhoCorasick::PrintTermsForCurrentState(unsigned int currentPos) const {
    
    unsigned int strNumb;
    unsigned int posNumb;
    
    if(DEBUG_STACK_ACT) {
        std::stack<TAKNode*> nodes;
        
        if (state->IsTerminal()) {
            if(DEBUG_WORK_WITH_STACK) {
                for(int i = 0; i < state->GetCount(); ++i) {
                    std::cout << "Number = " << state->numb[i] << " pushed in stack" << std::endl;
                }
            }
            nodes.push(state);
        }
        TAKNode* tmp = state->term;
        while (tmp) {
            nodes.push(tmp);
            tmp = tmp->term;
        }
        
        while(!nodes.empty()) {
            tmp = nodes.top();
            if(DEBUG_WORK_WITH_STACK) {
                for(int i = 0; i < state->GetCount(); ++i) {
                    std::cout << "Number = " << state->numb[i] << " pushed from stack" << std::endl;
                }
            }
            nodes.pop();
            if(DEBUG_PARAM_FUN) {
                std::cout << "Current position in string = " << currentPos << std::endl;
                std::cout << "Number of string = " << strNumb << std::endl;
                std::cout << "Result:" << std::endl;
            }
            for(int i = 0; i < state->GetCount(); ++i) {
                std::cout << strNumb << ", " << currentPos - state->deep + 1 << ", " << state->numb[i] << std::endl;
            }
        }
    }
    else {
        
        
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
            for(int i = 0; i < state->GetCount(); ++i) {
                std::cout << strNumb << ", " << posNumb << ", " << state->numb[i] << std::endl;
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
            for(int i = 0; i < tmp->GetCount(); ++i) {
                std::cout << strNumb << ", " << posNumb << ", " << tmp->numb[i] << std::endl;
            }
            tmp = tmp->term;
        }
    }
}

void TAhoCorasick::Search(unsigned long long int numb, unsigned int strNumb, unsigned int currentPos) {
    if(DEBUG_SEARCH) {
        std::cout << "Search: " << std::endl;
        std::cout << "numb = " << numb << " | strNumb = " << strNumb << " | currentPos = " << currentPos << std::endl;
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
    PrintTermsForCurrentState(currentPos);
}


