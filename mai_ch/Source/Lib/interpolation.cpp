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

double TTableOfPoints::getP(size_t n, char t) {
    if(head == nullptr) {
        head = &begin;
        pos = 0;
    }

    n += 1;
    
    if(n > size) {
        std::cout << "Error: out of borders!" << std::endl;
        return 0;
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

    return (t == 'x') ? head->x : head->y;
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
}

TSplines::~TSplines() {}

void TSplines::Update() {
    size_t size = table->getSize() - 1;

    coef.clear();
    coef.resize(size);
    for(size_t i = 0; i < size; ++i) {
        coef[i].clear();
        coef[i].resize(4);
    }

    TMatrix SC(size - 1, size - 1, ZERO_MATRIX);
    TMatrix SB(size - 1, 1, ZERO_MATRIX);

    SC[0][0] = 2*(H(1) + H(2));
    SC[0][1] = H(2);
    SB[0][0] = 3 * ( ((table->getP(2, 'y') - table->getP(1, 'y')) / H(2)) - ((table->getP(1, 'y') - table->getP(0, 'y')) / H(1)) );
    
    for(size_t i = 3; i <= size - 1; ++i) {
        SC[i - 2][i - 3] = H(i - 1);
        SC[i - 2][i - 2] = 2 * (H(i - 1) + H(i));
        SC[i - 2][i - 1] = H(i);
        SB[i - 2][0]     = 3 * ( ((table->getP(i, 'y') - table->getP(i - 1, 'y')) / H(i)) - ((table->getP(i - 1, 'y') - table->getP(i - 2, 'y')) / H(i - 1)) );
    }

    SC[size - 2][size - 3] = H(size - 1);
    SC[size - 2][size - 2] = 2 * (H(size - 1) + H(size));
    SB[size - 2][0] = 3 * ( ((table->getP(size, 'y') - table->getP(size - 1, 'y')) / H(size)) - ((table->getP(size - 1, 'y') - table->getP(size - 2, 'y')) / H(size - 1)) );

    TMatrix C(size, 1), tmp;

    tmp = SweepMethod(SC, SB);

    for(size_t i = 1; i < size; ++i) {
        C[i][0] = tmp[i - 1][0];
    }
    
    for(size_t i = 1; i <= size; ++i) {
        coef[i - 1][0] = table->getP(i - 1, 'y');
        coef[i - 1][2] = C[i - 1][0];
    }

    for(size_t i = 1; i <= size - 1; ++i) {
        coef[i - 1][1] = ( ((table->getP(i, 'y') - table->getP(i - 1, 'y')) / H(i)) - ((H(i) * (C[i][0] + 2 * C[i - 1][0])) / 3) );
    }
    coef[size - 1][1] = ( ((table->getP(size, 'y') - table->getP(size - 1, 'y')) / H(size)) - ((H(size) * 2 * C[size - 1][0]) / 3) );

    for(size_t i = 1; i <= size - 1; ++i) {
        coef[i - 1][3] = (C[i][0] - C[i - 1][0]) / (3*H(i));
    }
    coef[size - 1][3] = (-C[size - 1][0]) / (3 * H(size));

    /*for(size_t i = 0; i < size; ++i) {
        for(size_t j = 0; j < 4; ++j) {
            std::cout << coef[i][j] << ' ';
        }
        std::cout << std::endl;
    }*/
    return;
}

inline double TSplines::H(size_t i) {
    return table->getP(i) - table->getP(i - 1);
}

double TSplines::operator () (double& x) {

    size_t p = 0;
    size_t size = table->getSize();
    
    while(x > table->getP(p) && p < size) {
        ++p;
    }

    if(p == 0 || p >= size) {
        std::cout << "Out of borders!" << std::endl;
        return 0;
    }

    return coef[p - 1][0] +
           coef[p - 1][1] * (x - table->getP(p - 1)) +
           coef[p - 1][2] * pow((x - table->getP(p - 1)), 2) +
           coef[p - 1][3] * pow((x - table->getP(p - 1)), 3);
}

void Derivative(TTableOfPoints* table, double x) {

    size_t p = 0;
    size_t size = table->getSize();
    
    while(x > table->getP(p) && p < size) {
        ++p;
    }   

    if(p == 0 || p >= size) {
        std::cout << "Out of borders!" << std::endl;
        return;
    }

    double x1 = table->getP(p - 1);
    double y1 = table->getP(p - 1, 'y');
    double x2 = table->getP(p);
    double y2 = table->getP(p, 'y');

    double der11 = (y2 - y1) / (x2 - x1);
    if(x != table->getP(p)) {
        std::cout << "1 derivative (1 approx): " << der11 << std::endl;
    }
    

    if(p + 1 < size) {
        double x3 = table->getP(p + 1);
        double y3 = table->getP(p + 1, 'y');

        double d1 = der11;
        double d2 = (y3 - y2) / (x3 - x2);

        if(x == table->getP(p)) {
            std::cout << "1 derivative (1 approx, left side): " << d1 << std::endl;
            std::cout << "1 derivative (1 approx, right side): " << d2 << std::endl;
        }
        
        double der12 = d1 + ((d2 - d1) / (x3 - x1)) * (2 * x - x1 - x2);

        std::cout << "1 derivative (2 approx): " << der12 << std::endl;

        double der22 = ((d2 - d1) / (x3 - x1)) * 2;

        std::cout << "2 derivative: " << der22 << std::endl;
    }
    
    return;
}

void Integral(double (*func)(double), double x1, double x2, double h1, double h2) {

    double x = x1;
    double i = 0;
    double i1, i2;

    std::cout << "X1 = " << x1 << ";" << std::endl;
    std::cout << "X2 = " << x2 << ";" << std::endl;
    std::cout << "h1 = " << h1 << ";" << std::endl;
    std::cout << "h2 = " << h2 << ";" << std::endl;
    
    while(x < x2) {
        i += h1 * func((x + x + h1) / 2);
        x += h1;
    }
    
    std::cout << "(Rectangle method, h1) I = " << i << std::endl;
    i1 = i;
    
    x = x1;
    i = 0;
    
    while(x < x2) {
        i += h2 * func((x + x + h2) / 2);
        x += h2;
    }
    std::cout << "(Rectangle method, h2) I = " << i << std::endl;
    i2 = i - i1;
    
    std::cout << "Error = " << (i2 > 0 ? i2 : -i2) << std::endl;

    
    x = x1;
    i = 0;

    while(x < x2) {
        i += func(x) + func(x + h1);
        x += h1;
    }

    i *= h1;
    i /= 2;
    std::cout << "(Trapezoid method, h1) I = " << i << std::endl;
    i1 = i;

    x = x1;
    i = 0;
    
    while(x < x2) {
        i += func(x) + func(x + h2);
        x += h2;
    }

    i *= h2;
    i /= 2;
    std::cout << "(Trapezoid method, h2) I = " << i << std::endl;
    i2 = i - i1;
    
    std::cout << "Error = " << (i2 > 0 ? i2 : -i2) << std::endl;
    
    while(x < x2) {
        i += func(x) + func(x + h1) + 4 * func(x + (h1 / 2));
        x += h1;
    }

    i *= h1;
    i /= 3;
    std::cout << "(Simpson method, h1)   I = " << i << std::endl;
    i1 = i;

    x = x1;
    i = 0;
    
    while(x < x2) {
        i += func(x) + func(x + h2) + 4 * func(x + (h2 / 2));
        x += h2;
    }

    i *= h2;
    i /= 3;
    std::cout << "(Simpson method, h2)   I = " << i << std::endl;
    i2 = i - i1;
    
    std::cout << "Error = " << (i2 > 0 ? i2 : -i2) << std::endl;
    return;
}


TMinSquare::TMinSquare() {
    power = 0;
    table = nullptr;
}

TMinSquare::TMinSquare(TTableOfPoints* nTable, size_t p) {
    power = p;
    table = nTable;
}

void TMinSquare::Errors() {
    double result, err = 0;
    for(size_t pos = 0; pos < table->getSize(); ++pos) {
        result = coef[0][0];
        for(size_t k = 1; k <= power; ++k) {
            result += coef[k][0]*pow((*table)[pos].first, k);
        }

        err += pow((*table)[pos].second - result, 2);
    }

    std::cout << "Error = " << err << std::endl;
    
    return;
}

void TMinSquare::Update() {

    size_t sM = power + 1;
    
    TMatrix A(sM, sM), B(sM, 1);
    A[0][0] = (*table).getSize();
    for(size_t i = 0; i < sM; ++i) {
        for(size_t j = 0; j < sM; ++j) {
            if(i != 0 || j != 0) {
                for(size_t k = 0; k < (*table).getSize(); ++k) {
                    A[i][j] += pow((*table)[k].first, i + j);
                }
            }
        }
    }

    for(size_t i = 0; i < sM; ++i) {        
        for(size_t k = 0; k < (*table).getSize(); ++k) {
            B[i][0] += (*table)[k].second * pow((*table)[k].first, i);
        }
    }

    std::pair<TMatrix, TMatrix> tmp = LUDecomposition(A, B, false);
    coef = SolveSystemViaLU(tmp.first, tmp.second);

    return;
}

double TMinSquare::operator () (double& x) {

    double result = coef[0][0];
    for(size_t k = 1; k <= power; ++k) {
        result += coef[k][0]*pow(x, k);
    }
    
    return result;
}

TMinSquare::~TMinSquare() {}
