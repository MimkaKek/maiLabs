#include "interpolation.hpp"

TTableItem::TTableItem(): x(0), y(0), prev(nullptr), next(nullptr) {}

TTableItem::TTableItem(double nx, double ny, TTableItem* nPrev, TTableItem* nNext): x(nx), y(ny), prev(nPrev), next(nNext) {}

TTableItem::~TTableItem() {
    if(next) {
        delete next;
    }
}

double TTableItem::GetY() {
    return y;
}

double TTableItem::GetX() {
    return x;
}

bool operator == (const TTableItem& a, const TTableItem& b) {
    return (a.x == b.x && a.y == b.y);
}

bool operator != (const TTableItem& a, const TTableItem& b) {
    return (a.x != b.x || a.y != b.y);
}

bool CX1 (TTableItem& a, TTableItem& b) {
    return a.GetX() > b.GetX();
}

bool CX2 (TTableItem& a, TTableItem& b) {
    return a.GetX() <= b.GetX();
}

bool CY1 (TTableItem& a, TTableItem& b) {
    return a.GetY() > b.GetY();
}

bool CY2 (TTableItem& a, TTableItem& b) {
    return a.GetY() <= b.GetY();
}

bool CC (TTableItem& a, TTableItem& b) {
    return true;
}

TTableOfPoints::TTableOfPoints(): size(0), pos(0), head(nullptr), end(nullptr), Compare(CX1) {}

TTableOfPoints::~TTableOfPoints() {}

void TTableOfPoints::SetCompare(bool (*n)(TTableItem&, TTableItem&)) {
    Compare = n;
    return;
}

void TTableOfPoints::SetHead(size_t n) {
    if(head == nullptr) {
        head = &begin;
        pos = 0;
    }
    
    if(n > size) {
        std::cout << "Error: out of borders!" << std::endl;
        return;
    }

    size_t delta = n - pos;
    if(delta >= 0) {
        if(delta > (size / 2)) {
            head = end;
            pos = size;
        }
    }
    else {
        if(delta < -(size / 2)) {
            head = &begin;
            pos = 0;
        }
    }

    delta = n - pos;
    if(delta >= 0) {
        while(pos != n) {
            head = head->next;
            ++pos;
        }
    }
    else {
        while(pos != n) {
            head = head->prev;
            --pos;
        }
    }

    return;
}

void TTableOfPoints::AddPoint(double nx, double ny) {

    TTableItem* toAdd = new TTableItem(nx, ny, nullptr, nullptr);
    TTableItem* pItem = &begin;
    TTableItem* cItem = begin.next;
    size_t n = 1;
    while(cItem != nullptr && Compare(*toAdd, *cItem)) {
        ++n;
        pItem = cItem;
        cItem = cItem->next;
    }

    if(cItem != nullptr) {
        if(*toAdd == *cItem) {
            std::cout << "Table already have those points " << nx << " and " << ny << std::endl;
            delete toAdd;
            return;
        }
    }
    
    
    toAdd->prev = pItem;
    toAdd->next = cItem;
    pItem->next = toAdd;
    if(cItem != nullptr) {
        cItem->prev = toAdd;
    }
    else {
        end = toAdd;
    }

    if(pos > n) {
        ++pos;
    }
    
    ++size;

    return;
}

void TTableOfPoints::RemovePoint(size_t n) {

    if(n > size) {
        std::cout << "Error: out of borders!" << std::endl;
        return;
    }

    long long   delta[3];
    delta[0] = n - 1;
    delta[1] = n - pos;
    delta[2] = n - size;

    long long   mDelta = delta[0];
    long long   aDelta;

    TTableItem* pItem;
    TTableItem* cItem;

    size_t      dPos;
    size_t      c = 0;

    for(size_t p = 1; p < 3; ++p) {
        aDelta = (delta[p] < 0     ? -delta[p] : delta[p]);
        if(mDelta > aDelta) {
            mDelta = aDelta;
            c = p;
        }
    }

    switch(c) {
        case 0:
            pItem = &begin;
            cItem = begin.next;
            dPos = 1;
            break;
        case 1:
            pItem = head->prev;
            cItem = head;
            dPos = pos;
            break;
        case 2:
            pItem = end->prev;
            cItem = end;
            dPos = size;
            break;
    }

    if(delta[c] >= 0) {
        while(dPos != n) {
            pItem = cItem;
            cItem = cItem->next;
            ++dPos;
        }
    }
    else {
        while(dPos != n) {
            cItem = pItem;
            pItem = pItem->prev;
            --dPos;
        }
    }
    
    pItem->next = cItem->next;
    if(cItem->next != nullptr) {
        cItem->next->prev = pItem;
    }
    else {
        end = pItem;
    }
    
    cItem->next = nullptr;

    delete cItem;

    if(pos == n) {
        head = pItem;
    }

    if(pos >= n) {
        --pos;
    }
    
    --size;

    return;
}

void TTableOfPoints::PrintTable() {
    
    std::cout << " <<TABLE>> " << std::endl;
    TTableItem* cItem = begin.next;
    if(size == 0) {
        std::cout << "   Empty   " << std::endl;
        return;
    }

    size_t n = 1;
    while(cItem != nullptr) {
        if(head == cItem) {
            std::cout << '<' << n << "> ";
        }
        else {
            std::cout << '|' << n << "| ";
        }
        std::cout << cItem->x << " -> " << cItem->y << std::endl;
        cItem = cItem->next;
        ++n;
    }
    
    return;
}

bool TTableOfPoints::IsEmpty() {
    return size == 0;
}

size_t TTableOfPoints::getSize() {
    return size;
}

std::pair<double, double> TTableOfPoints::operator [] (const size_t n) {
    if(head == nullptr) {
        head = &begin;
        pos = 0;
    }
    
    if(n > size) {
        std::cout << "Error: out of borders!" << std::endl;
        return std::pair<double, double>(0, 0);
    }

    long long delta = n - pos;
    if(delta >= 0) {
        if(delta > (size / 2)) {
            head = end;
            pos = size;
        }
    }
    else {
        if(delta < -(size / 2)) {
            head = &begin;
            pos = 0;
        }
    }

    delta = n - pos;
    if(delta >= 0) {
        while(pos != n) {
            head = head->next;
            ++pos;
        }
    }
    else {
        while(pos != n) {
            head = head->prev;
            --pos;
        }
    }

    return std::pair<double, double>(head->x, head->y);
}

std::ostream& operator << (std::ostream &out, const TTableOfPoints &table) {

    out << " <<TABLE>> " << std::endl;
    TTableItem* cItem = table.begin.next;
    if(table.size == 0) {
        out << "   Empty   " << std::endl;
        return out;
    }

    size_t n = 1;
    while(cItem != nullptr) {
        if(table.head == cItem) {
            out << '<' << n << "> ";
        }
        else {
            out << '|' << n << "| ";
        }
        out << cItem->x << " -> " << cItem->y << std::endl;
        cItem = cItem->next;
        ++n;
    }
    
    return out;
}

TLagrangeInter::TLagrangeInter(): table(nullptr) {}

TLagrangeInter::TLagrangeInter(TTableOfPoints* nTable) {
    table = nTable;
    
    size_t size = table->getSize();
    double nCoef;
    for(size_t i = 1; i <= size; ++i) {
        nCoef = (*table)[i].second;
        for(size_t j = 1; j <= size; ++j) {
            if(i != j) {
                nCoef /= ( (*table)[i].first - (*table)[j].first );
            }
        }
        coef.push_back(nCoef);
    }
}

TLagrangeInter::~TLagrangeInter() {}

void TLagrangeInter::Update() {
    coef.clear();
    
    size_t size = table->getSize();
    double nCoef;
    for(size_t i = 1; i <= size; ++i) {
        nCoef = (*table)[i].second;
        for(size_t j = 1; j <= size; ++j) {
            if(i != j) {
                nCoef /= ( (*table)[i].first - (*table)[j].first );
            }
        }
        coef.push_back(nCoef);
    }

    return;
}

double TLagrangeInter::operator () (double& x) {
    size_t size = coef.size();
    double y = 0;
    double fx;
    for(size_t i = 1; i <= size; ++i) {
        fx = coef[i - 1];
        for(size_t j = 1; j <= size; ++j) {
            if(i != j) {
                fx *= (x - (*table)[j].first);
            }
        }
        y += fx;
    }

    return y;
}

TNewtonInter::TNewtonInter(): table(nullptr) {}

TNewtonInter::TNewtonInter(TTableOfPoints* nTable) {
    table = nTable;
    
    size_t size = table->getSize();

    coef.clear();
    coef.resize(size);
    for(size_t i = 1; i <= size; ++i) {
        coef[0].push_back((*table)[i].second);
    }

    size_t nf;
    for(size_t i = 1; i < size; ++i) {
        nf = size - i; 
        for(size_t j = 0; j < nf; ++j) {
            coef[i].push_back(coef[i - 1][j + 1] - coef[i - 1][j]);
        }
    }
}

TNewtonInter::~TNewtonInter() {}

void TNewtonInter::Update() {
    size_t size = table->getSize();

    coef.clear();
    coef.resize(size);
    for(size_t j = 1; j <= size; ++j) {
        coef[0].push_back((*table)[j].second);
    }
    
    size_t nf;
    for(size_t i = 1; i < size; ++i) {
        nf = size - i;
        for(size_t j = 0; j < nf; ++j) {
            coef[i].push_back((coef[i - 1][j + 1] - coef[i - 1][j]) /
                              (((*table)[j + i + 1].first) - ((*table)[j + 1].first)));
        }
    }
}

double TNewtonInter::operator () (double& x) {
    size_t size = coef.size();
    double y = 0;
    double fx;
    for(size_t i = 0; i < size; ++i) {
        fx = coef[i][0];
        for(size_t j = i; j > 0; --j) {
                fx *= (x - (*table)[j].first);
        }
        y += fx;
    }

    return y;
}

TSplines::TSplines(TTableOfPoints* nTable) {
    table = nTable;
    
    size_t size = table->getSize();

    coef.clear();
    coef.resize(size);
    for(size_t i = 1; i <= size; ++i) {
        coef[0].push_back((*table)[i].second);
    }

    size_t nf;
    for(size_t i = 1; i < size; ++i) {
        nf = size - i; 
        for(size_t j = 0; j < nf; ++j) {
            coef[i].push_back(coef[i - 1][j + 1] - coef[i - 1][j]);
        }
    }
}

TSplines::~TSplines() {}

void TSplines::Update() {
    size_t size = table->getSize();

    coef.clear();
    coef.resize(size);
    for(size_t j = 1; j <= size; ++j) {
        coef[0].push_back((*table)[j].second);
    }
    
    size_t nf;
    for(size_t i = 1; i < size; ++i) {
        nf = size - i;
        for(size_t j = 0; j < nf; ++j) {
            coef[i].push_back((coef[i - 1][j + 1] - coef[i - 1][j]) /
                              (((*table)[j + i + 1].first) - ((*table)[j + 1].first)));
        }
    }
}

double TSplines::operator () (double& x) {
    size_t size = coef.size();
    double y = 0;
    double fx;
    for(size_t i = 0; i < size; ++i) {
        fx = coef[i][0];
        for(size_t j = i; j > 0; --j) {
                fx *= (x - (*table)[j].first);
        }
        y += fx;
    }

    return y;
}
