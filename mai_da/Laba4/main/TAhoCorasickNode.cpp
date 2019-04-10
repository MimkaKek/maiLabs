#include "TAhoCorasickNode.h"

TAKNode::TAKNode() {
    fail = nullptr;
    term = nullptr;
    deep = 0;
}

TAKNode::TAKNode(TAKNode* failLink) {
    fail = failLink;
    term = nullptr;
    deep = 0;
}

TAKNode* TAKNode::GetLink(unsigned long long int symb) const {
    TLinksMap::const_iterator iter = links.find(symb);
    return (iter != links.cend()) ? iter->second : nullptr;
}

int TAKNode::GetCount() const {
    return numb.size();
}

void TAKNode::PushNumber(int number) {
    numb.push_back(number);
    return;
}

bool TAKNode::IsTerminal() const {
    return !(numb.empty());
}
