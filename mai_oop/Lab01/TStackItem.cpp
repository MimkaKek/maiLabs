#include "TStackItem.h"
#include <iostream>

TStackItem::TStackItem(const Rhombus& rhombus) {
    this->rhombus = rhombus;
    this->next = nullptr;
    std::cout << "Stack item: created" << std::endl;
}

TStackItem::TStackItem(const TStackItem& orig) {
    this->rhombus = orig.rhombus;
    this->next = orig.next;
    std::cout << "Stack item: copied" << std::endl;
}

TStackItem* TStackItem::SetNext(TStackItem* next) {
    TStackItem* old = this->next;
    this->next = next;
    return old;
}

Rhombus TStackItem::GetRhombus() const {
    return this->rhombus;
}

TStackItem* TStackItem::GetNext() {
    return this->next;
}

TStackItem::~TStackItem() {
    std::cout << "Stack item: deleted" << std::endl;
    delete next;

}

std::ostream& operator<<(std::ostream& os, const TStackItem& obj) {
    os << "[" << obj.rhombus << "]" << std::endl;
    return os;
}
