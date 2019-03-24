#include "TAhiKorasikNode.h"

TAKNode::TAKNode() {
    fail = nullptr;
    term = nullptr;
    out = -1;
}

TAKNode::TAKNode(TAKNode* failLink) {
    fail = failLink;
    term = nullptr;
    out = -1;
}

TAKNode* TAKNode::GetLink(char symb) const {
    LinksMap::const_iterator iter = links.find(symb);
    return (iter != links.cend()) ? iter->second : nullptr;
}

bool TAKNode::IsTerminal() const {
    return (out >= 0);
}
