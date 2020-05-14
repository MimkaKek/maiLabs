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

class TList {
    private:
    
        class TListItem {
            private:
            
                TListItem*          next;
                unsigned char       n;

            public:
                TListItem() {
                    next = nullptr;
                    n = 0;
                };
    
                TListItem(char nletter, TListItem* item) {
                    n = nletter;
                    next = item;
                };

                ~TListItem() {
                    if(next) {
                        delete next;
                    }
                }

                TListItem* getNext() {
                    return next;
                };

                void setNext(TListItem* item) {
                    next = item;
                    return;
                };

                char getLetter() {
                    return n;
                };
                
        };

        TListItem*      begin;
        
    public:

        TList() {
            begin = new TListItem(END, nullptr);
        };
        
        void addItem(char letter) {
            
            TListItem* curr = begin->getNext();
            TListItem* prev = begin;
            
            while(curr != nullptr && letter > curr->getLetter()) {
                prev = curr;
                curr = curr->getNext();
            }
            
            TListItem* newItem = new TListItem(letter, curr);
            prev->setNext(newItem);
            return;
        };

        unsigned char popItem() {
            TListItem* toDelete = begin->getNext();

            if(toDelete == nullptr) {
                return END;
            }
            
            unsigned char letter = toDelete->getLetter();
            letter = (letter == SENTINEL) ? GET_SENTINEL : letter - 'a';
            begin->setNext(toDelete->getNext());
            toDelete->setNext(nullptr);
            delete toDelete;
            return letter;
        };

        ~TList() {
            delete begin;
        }
};

class TNode {
    public:
        TNode*                                      edges[ALPHA_SIZE];
        TList                                       list;
        size_t                                      left;
        size_t*                                     right;
        size_t                                      number;
        TNode*                                      suffLink;

        TNode(size_t l, size_t *r, size_t n, TNode *node) : edges{}, left(l), right(r), number(n), suffLink(node) {
        }

        TNode* getEdge(char letter) {
            size_t pos = (letter == SENTINEL) ? 26 : letter - 'a';
            return edges[pos];
        };

        void setEdge(char letter, TNode* node) {
            size_t pos = (letter == SENTINEL) ? 26 : letter - 'a';
            edges[pos] = node;
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

        void addLetter(char letter) {
            list.addItem(letter);
            return;
        };

        unsigned char getLetter() {
            return list.popItem();
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
                    activeNode->addLetter(str[activeEdge]);
        
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

            unsigned char pos = root->getLetter();
            if(pos == GET_SENTINEL) {
                pos = root->getLetter();
            }

            while(pos != END) {
                
                if(GetLowest(root->edges[pos]->getLength(), root->edges[pos])) {
                        std::cout << answer << std::endl;
                        return;
                }

                pos = root->getLetter();
            }
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
        
        bool GetLowest(size_t strLength, TNode* node) {

            if(strLength >= (str.length() / 2)) {
                size_t delta = strLength - (str.length() / 2);
                for(size_t p = node->getRight() - delta; p >= node->getLeft(); --p) {
                    answer[ansPos] = str[p];
                    --ansPos;
                }
                return true;
            }


            unsigned char pos = node->getLetter();
            if(pos == GET_SENTINEL) {
                pos = node->getLetter();
            }

            while(pos != END) {
                
                if(GetLowest(strLength + node->edges[pos]->getLength(), node->edges[pos])) {
                    for(size_t p = node->getRight(); p >= node->getLeft(); --p) {
                        answer[ansPos] = str[p];
                        --ansPos;
                        
                        if(p == 0) {
                            break;
                        }
                    }
                    return true;
                }

                pos = node->getLetter();
            }
            return false;
        };
        
        TNode* Insert(TNode *toInsert) {
            TNode *edge = activeNode->getEdge(str[activeEdge]);
            size_t left = edge->getLeft();
            
            TNode *newInternalNode = new TNode(left, (size_t *)(left + activeLength - 1), INTERNAL, root);

            newInternalNode->setEdge(str[left + activeLength], edge);
            newInternalNode->addLetter(str[left + activeLength]);

            edge->left = left + activeLength;

            newInternalNode->setEdge(str[toInsert->left], toInsert);
            newInternalNode->addLetter(str[toInsert->left]);

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
