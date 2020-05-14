#include "sffxtr.h"

TSuffTree::TSuffTree(const std::string &s) : str(std::move(s + SENTINEL)) {
    root = new TNode(0, 0, INTERNAL, nullptr);
    
    
    activeNode = root;
    activeEdge = 0;
    activeLength = 0;

    remain = 0;

    end = -1;
    suffNum = 0;

    size_t len = str.length();

    answer = new char[(len / 2) + 1];
    ansPos = len / 2;
    answer[ansPos] = '\0';
    --ansPos;

    for (size_t i = 0; i < len; i++) {
        Phase(i);
    }
}
    
TSuffTree::~TSuffTree() {
    DeleteTree(root);
}

void TSuffTree::Phase(size_t phaseNum) {
    
    remain++;       //узел, который надо вставить в дерево
    end++;          //позиция конца строки

    TNode *lastAddedInternalNode = nullptr;

    while (remain > 0) {

        if (activeLength == 0) {
            // Если такой суффикс ещё не встречался - номер фазы есть номер символа, с которого он начинается
            activeEdge = phaseNum;
        }
        if (activeNode->getEdge(str, activeEdge) == nullptr) {
            // Если нет ребра с таким символом - создаём лист
            activeNode->getEdge(str, activeEdge) = new TNode(phaseNum, &end, suffNum, nullptr);

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
}

void TSuffTree::Task() {

    root->genVector();
    
    if(!root->sEdges.empty()) {
        for(size_t i = 0; i < root->sEdges.size(); ++i) {
            if(root->sEdges[i]->first != '$') {
                if(GetLowest((root->sEdges[i])->second->getLength(), (root->sEdges[i])->second)) {
                    std::cout << answer << std::endl;
                    return;
                }
            }
        }
    }
    return;
}

bool TSuffTree::GetLowest(size_t strLength, TNode* pos) {

    if(strLength >= (str.length() / 2)) {
        size_t delta = strLength - (str.length() / 2);
        for(size_t p = pos->getRight() - delta; p >= pos->getLeft(); --p) {
            answer[ansPos] = str[p];
            --ansPos;
        }
        return true;
    }
    
    pos->genVector();
    
    if(!pos->sEdges.empty()) {
        for(size_t i = 0; i < pos->sEdges.size(); ++i) {
            if(pos->sEdges[i]->first != '$') {
                if(GetLowest( strLength + (pos->sEdges[i])->second->getLength(), (pos->sEdges[i])->second)) {
                    for(size_t p = pos->getRight(); p >= pos->getLeft(); --p) {
                        answer[ansPos] = str[p];
                        --ansPos;
                        if(p == 0) {
                            break;
                        }
                    }
                    return true;
                }
            }
    
            
        }
    }

    return false;
}

int TSuffTree::Walkdown() {
    TNode *edge = activeNode->getEdge(str, activeEdge);
    size_t len = edge->getLength();
    if (activeLength >= len) {
        activeLength -= len;        
        activeEdge += len;
        activeNode = edge;
        return 1;
    }
    return 0;
}

char TSuffTree::getNextCharAct() {
    size_t ind = activeNode->getEdge(str, activeEdge)->left + activeLength;
    return str[ind];
}

TNode* TSuffTree::Insert(TNode *toInsert) {
    TNode *edge = activeNode->getEdge(str, activeEdge);
    size_t left = edge->getLeft();
    TNode *newInternalNode = new TNode(left, (size_t *)(left + activeLength - 1), INTERNAL, root);
    newInternalNode->getEdge(str, left + activeLength) = edge;
    edge->left = left + activeLength;
    newInternalNode->getEdge(str, toInsert->left) = toInsert;
    activeNode->getEdge(str, activeEdge) = newInternalNode;
    return newInternalNode;
}

void TSuffTree::DeleteTree(TNode *node) {
    if (!node) {
        return;
    }
    
    if (node->number != INTERNAL) {
        delete node;
        return;
    }
    
    for (auto it = node->edges.begin(); it != node->edges.end(); ++it) {
        DeleteTree(it->second);
    }
    
    delete node;
}

void TSuffTree::Print() {
    std::cout << "root" << std::endl;
    
    for (auto it = root->edges.cbegin(); it != root->edges.cend(); ++it) {
        print(it->second, 3);
    }
    std::cout << "suffix links: " << std::endl;
    for (auto it = root->edges.cbegin(); it != root->edges.cend(); ++it) {
        print_suff_links(it->second);
    }
}

void TSuffTree::print(TNode *node, int tab) {
    const int tabIncr = 3;
    if (node == nullptr)
        return;
    for (int i = 0; i < tab; i++) {
        std::cout << ' ';
    }
    node->Print(str);
    std::cout << std::endl;
    for (auto it = node->edges.cbegin(); it != node->edges.cend(); ++it) {
        print(it->second, tab + tabIncr);
    }
}

void TSuffTree::print_suff_links(TNode *node) {
    if (node == nullptr)
        return;
    if (node->suffLink != nullptr) {
        std::cout << "[ " << node->getLeft() << " , " << node->getRight() << " ]"
                  << " --> ";
        TNode *sl = node->suffLink;
        if (sl == root) {
            std::cout << "ROOT";
        } else if (sl == nullptr) {
            std::cout << "NULL";
        } else {
            std::cout << "[ " << sl->getLeft() << " , " << sl->getRight() << " ]";
        }
        std::cout << std::endl;
    }
    for (auto it = node->edges.cbegin(); it != node->edges.cend(); ++it) {
        print_suff_links(it->second);
    }
}
