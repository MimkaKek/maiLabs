#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <utility>
#include <map>
#include <cstdint>
#include <ctime>

const unsigned char     END           = 255;
const unsigned char     GET_SENTINEL  = 254;
const size_t            ALPHA_SIZE    = 27;
const size_t            INTERNAL      = SIZE_MAX;
const char              SENTINEL      = '$';

class TNode {
    public:
        TNode*                                      edges[ALPHA_SIZE];
        size_t                                      left;
        size_t*                                     right;
        size_t                                      number;
        size_t                                      bSearch;
        TNode*                                      suffLink;

        TNode(size_t l, size_t *r, size_t n, TNode *node) : edges{}, left(l), right(r), number(n), bSearch(ALPHA_SIZE), suffLink(node) {
        }

        TNode* getEdge(char letter) {
            size_t pos = (letter == SENTINEL) ? 26 : letter - 'a';
            return edges[pos];
        };

        void setEdge(char letter, TNode* node) {
            size_t pos = (letter == SENTINEL) ? 26 : letter - 'a';
            edges[pos] = node;
            bSearch = (bSearch > pos) ? pos : bSearch;
            return;
        };
        
        size_t getLeft() {
            return left;
        };
        
        size_t getRight() {
            return number == INTERNAL ? (size_t)right : *right;
        };
        
        size_t getLength() {
            return getRight() - getLeft() + 1;
        };
};

class TSuffTree {
    public:
        TSuffTree(const std::string &s) : str(std::move(s + SENTINEL)) {

            root = new TNode(0, 0, INTERNAL, nullptr);
            
            
            activeNode = root;
            activeEdge = 0;
            activeLength = 0;
        
            remain = 0;
        
            end = -1;
            suffNum = 0;
        
            size_t len = str.length();
        
            answer = (char*) malloc((sizeof (char)) * (len / 2) + 1);
            ansPos = len / 2;
            answer[ansPos] = '\0';
            --ansPos;
            for (size_t i = 0; i < len; i++) {
                Phase(i);
            }
        };
        
        ~TSuffTree() {
            DeleteTree(root);
        };
        
        void Phase(size_t phaseNum) {
            
            remain++;       //узел, который надо вставить в дерево
            end++;          //позиция конца строки
        
            TNode *lastAddedInternalNode = nullptr;
        
            while (remain > 0) {
        
                if (activeLength == 0) {
                    // Если такой суффикс ещё не встречался - номер фазы есть номер символа, с которого он начинается
                    activeEdge = phaseNum;
                }
                if (activeNode->getEdge(str[activeEdge]) == nullptr) {
                    // Если нет ребра с таким символом - создаём лист
                    activeNode->setEdge(str[activeEdge], new TNode(phaseNum, &end, suffNum, nullptr));
        
                    suffNum++;
                    remain--;
        
                    // Не забываем про суффиксные ссылки
                    if (lastAddedInternalNode != nullptr) {  
                        lastAddedInternalNode->suffLink = activeNode;
                        lastAddedInternalNode = nullptr;
                    }
                    
                } else {
                    // Если имеется ребро - спускаемся по нему
                    if (Walkdown()) {
                        // Если мы попали на следующий узел или у нас текущий суффикс длинее - двигаемся по дереву дальше
                        continue;
                    }
        
                    
                    if (getNextCharAct() == str[phaseNum]) {
                        // Если у следующего узла есть ребро с нужным символом - вставка не требуется
                        if (lastAddedInternalNode != nullptr && activeNode != root) {
                            lastAddedInternalNode->suffLink = activeNode;
                            lastAddedInternalNode = nullptr;
                        }
                        activeLength++;
                        break;
                    } else {
                        // Иначе создаём промежуточный узел
                        TNode *toInsert = new TNode(phaseNum, &end, suffNum, nullptr);
                        suffNum++;
        
                        TNode *justInserted = Insert(toInsert);
                        if (lastAddedInternalNode != nullptr) {
                            lastAddedInternalNode->suffLink = justInserted;
                        }
                        lastAddedInternalNode = justInserted;
                        remain--;
                    }
                }
        
                
                if (activeNode == root) {
                    if (activeLength > 0) {
                        activeLength--;
                        activeEdge = phaseNum - remain + 1;
                    }
                }
                else {
                    activeNode = activeNode->suffLink;
                }
            }
        };
        
        void Task() {

            std::stack<TNode*> path;
            TNode*             state = root;
            size_t             len = 0;
            size_t             needed = (str.length() - 1) / 2;
            size_t             p;
            bool               cont = false;

            while(true) {
                while(len < needed) {
                    
                    for(size_t p = state->bSearch; p < ALPHA_SIZE; ++p) {
                        if(state->edges[p]) {
                            path.push(state);
                            state->bSearch = p + 1;
                            state = state->edges[p];
                            len += state->getLength();
                            cont = true;
                            break;
                        }
                    }
                    
                    if(cont) {
                        cont = false;
                        continue;
                    }
                    
                    len -= state->getLength();
                    state = path.top();
                    path.pop();
                }

                p = state->getRight() - len + needed;
                if(str[p] == '$') {
                    len -= state->getLength();
                    state = path.top();
                    path.pop();
                    continue;
                }
                
                while(true) {
                    answer[ansPos] = str[p];
                    if(ansPos == 0) {
                        break;
                    }
                    --ansPos;
                    --p;
                }

                break;
            }
            std::cout << answer << std::endl;
            
            return;
        };
        
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
        
        
        
        TNode* Insert(TNode *toInsert) {
            TNode *edge = activeNode->getEdge(str[activeEdge]);
            size_t left = edge->getLeft();
            
            TNode *newInternalNode = new TNode(left, (size_t *)(left + activeLength - 1), INTERNAL, root);

            newInternalNode->setEdge(str[left + activeLength], edge);

            edge->left = left + activeLength;

            newInternalNode->setEdge(str[toInsert->left], toInsert);

            activeNode->setEdge(str[activeEdge], newInternalNode);

            return newInternalNode;
        };

        int Walkdown() {
            TNode *edge = activeNode->getEdge(str[activeEdge]);
            size_t len = edge->getLength();
            if (activeLength >= len) {
                activeLength -= len;        
                activeEdge += len;
                activeNode = edge;
                return 1;
            }
            return 0;
        };
        
        char getNextCharAct() {
            size_t ind = activeNode->getEdge(str[activeEdge])->left + activeLength;
            return str[ind];
        };
        
        void DeleteTree(TNode *node) {

            if (!node) {
                return;
            }
            
            if (node->number != INTERNAL) {
                delete node;
                return;
            }
            
            for (size_t pos = 0; pos < ALPHA_SIZE; ++pos) {
                DeleteTree(node->edges[pos]);
            }
            
            delete node;
        };
};

int main() {
    std::string pattern;
    std::cin >> pattern;
    //clock_t begin = clock();
    TSuffTree tree(pattern + pattern);
    tree.Task();
    //clock_t end = clock();
    //float time = (end - begin) /  (float) CLOCKS_PER_SEC;
    //std::cout << "Total time = " << time << std::endl;
    return 0;
}
