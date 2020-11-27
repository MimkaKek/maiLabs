#include "matrix.hpp"

TMatrix::TMatrix(): trans(false), own(true), a(0), b(0), m(nullptr) {}

TMatrix::TMatrix(const TMatrix &orig) {

    rp    = orig.rp;
    trp   = orig.trp;
    trans = orig.trans;
    own   = true;
    
    a = orig.a;
    b = orig.b;
    
    m = new double*[a];
    for(size_t i = 0; i < a; ++i) {
        m[i] = new double[b];
    }

    for(size_t i = 0; i < a; ++i) {
        for(size_t j = 0; j < b; ++j) {
            m[i][j] = orig.m[i][j];
        }
    }
}

TMatrix::TMatrix(const TMatrix &orig, bool t) {

    trans = t;
    own = false;
    
    a = orig.a;
    b = orig.b;
    
    m = orig.m;
}

TMatrix::TMatrix(const TMatrix &orig, size_t i1, size_t j1, size_t i2, size_t j2) {

    a = i2 - i1 + 1;
    b = j2 - j1 + 1;
    
    trans = 0;
    own = true;
    m = new double*[a];
    for(size_t i = 0; i < a; ++i) {
        m[i] = new double[b];
    }

    for(size_t soi = i1, si = 0; soi <= i2; ++soi, ++si) {
        for(size_t soj = j1, sj = 0; soj <= j2; ++soj, ++sj) {
            m[si][sj] = orig.m[soi][soj];
        }
    }
    
}

TMatrix::TMatrix(std::istream &is) {

    std::cout << "Enter the number of lines: ";
    is >> a;
    std::cout << "Enter the number of columns: ";
    is >> b;
    
    trans = false;
    own = true;
    
    m = new double*[a];
    for(size_t i = 0; i < a; ++i) {
        m[i] = new double[b];
    }
    
    std::cout << "Enter items of matrix:" << std::endl;
    for(size_t i = 0; i < a; ++i) {
        for(size_t j = 0; j < b; ++j) {
            is >> m[i][j];
        }
    }
}

TMatrix::TMatrix(size_t na, size_t nb, double **matrix, bool copy) {

    a = na;
    b = nb;

    trans = false;
    own = true;

    if(copy) {
        if(matrix == nullptr) {
            std::cout << "Error: matrix constructor get nullptr!" << std::endl;
            exit(1);
        }
        else {

            m = new double*[a];
            for(size_t i = 0; i < a; ++i) {
                m[i] = new double[b];
            }
            
            for(size_t i = 0; i < a; ++i) {
                for(size_t j = 0; j < b; ++j) {
                    m[i][j] = matrix[i][j];
                }
            }
        }
    }
    else {
        m = matrix;
    }
    
    
}

TMatrix::TMatrix(size_t na, size_t nb, size_t type) {

    a = na;
    b = nb;
    
    trans = false;
    own = true;
    
    m = new double*[a];
    for(size_t i = 0; i < a; ++i) {
        m[i] = new double[b];
    }

    switch(type) {
        case ZERO_MATRIX:
            for(size_t i = 0; i < a; ++i) {
                for(size_t j = 0; j < b; ++j) {
                    m[i][j] = 0;
                }
            }
            break;
        case UNIT_MATRIX:
            for(size_t i = 0; i < a; ++i) {
                for(size_t j = 0; j < b; ++j) {
                    m[i][j] = (i == j ? 1 : 0);
                }
            }
            break;
        default:
            for(size_t i = 0; i < a; ++i) {
                for(size_t j = 0; j < b; ++j) {
                    m[i][j] = 0;
                }
            }
            break;
    }
}

TMatrix::~TMatrix() {
    if(m != nullptr && own) {
        for(size_t i = 0; i < a; ++i) {
            delete m[i];
        }
        delete m;
    }
}

double TMatrix::CalcNorm(unsigned char type) {
    
    if(a == 1) {
        switch(type) {
            case TYPE_1:
            {
                double norm = 0;
                for(size_t j = 0; j < b; ++j) {
                    norm += (m[0][j] < 0) ? -m[0][j] : m[0][j];
                }
                return norm;
            }
            case TYPE_2:
            {
                double norm = 0;
                for(size_t j = 0; j < b; ++j) {
                    norm += m[0][j] * m[0][j];
                }

                norm = sqrt(norm);
                
                return norm;
            }
            case TYPE_C:
            {
                double norm = m[0][0];
                for(size_t j = 0; j < b; ++j) {
                    norm = (norm < m[0][j]) ? m[0][j] : norm;
                }
                return norm;
            }
            default:
                std::cout << "Error: Can't calculate this type of norm of vector!" << std::endl;
                exit(1);
        }
    }
    else if(b == 1) {
        switch(type) {
            case TYPE_1:
            {
                double norm = 0;
                for(size_t i = 0; i < a; ++i) {
                    norm += (m[i][0] < 0) ? -m[i][0] : m[i][0];
                }
                return norm;
            }
            case TYPE_2:
            {
                double norm = 0;
                for(size_t i = 0; i < a; ++i) {
                    norm += m[i][0] * m[i][0];
                }

                norm = sqrt(norm);
                
                return norm;
            }
            case TYPE_C:
            {
                double norm = m[0][0];
                for(size_t i = 0; i < a; ++i) {
                    norm = (norm < m[i][0]) ? m[i][0] : norm;
                }
                return norm;
            }
            default:
                std::cout << "Error: Can't calculate this type of norm of vector!" << std::endl;
                exit(1);
        }
    }
    else {
        switch(type) {
            case TYPE_1:
            {
                double norm = 0;
                double nextNorm;

                for(size_t i = 0; i < a; ++i) {
                    norm += (m[i][0] < 0) ? -m[i][0] : m[i][0];
                }

                for(size_t j = 1; j < b; ++j) {

                    nextNorm = 0;
                    for(size_t i = 0; i < a; ++i) {
                        nextNorm += (m[i][j] < 0) ? -m[i][j] : m[i][j];
                    }

                    norm = (norm < nextNorm) ? nextNorm : norm;
                }
                
                return norm;
            }
            case TYPE_2:
            {
                double norm = 0;
                for(size_t i = 0; i < a; ++i) {
                    for(size_t j = 0; j < b; ++j) {
                        norm += m[i][j] * m[i][j];
                    }
                }

                norm = sqrt(norm);
                
                return norm;
            }
            case TYPE_C:
            {
                double norm = 0;
                double nextNorm;

                for(size_t j = 0; j < b; ++j) {
                    norm += (m[0][j] < 0) ? -m[0][j] : m[0][j];
                }

                for(size_t i = 1; i < a; ++i) {

                    nextNorm = 0;
                    for(size_t j = 0; j < b; ++j) {
                        nextNorm += (m[i][j] < 0) ? -m[i][j] : m[i][j];
                    }

                    norm = (norm < nextNorm) ? nextNorm : norm;
                }
                
                return norm;
            }
            default:
                std::cout << "Error: Can't calculate this type of norm of matrix!" << std::endl;
                exit(1);
        }
    }
}

void TMatrix::SwapInMatrix(size_t i1, size_t i2, unsigned char type) {
    
    if(type == STRING) {
        double* tmp = m[i1];
        m[i1] = m[i2];
        m[i2] = tmp;  
    }
    else {
        double tmp;
        for(size_t i = 0; i < a; ++i) {
            tmp = m[i][i1];
            m[i][i1] = m[i][i2];
            m[i][i2] = tmp;
        }
    }
    trp.push_back(type);
    rp.push_back(std::pair<size_t, size_t>(i1, i2));
    return;
}

void TMatrix::SwapBackward(unsigned char type) {
    for(size_t pos = rp.size() - 1; pos >= 0; --pos) {
        if(trp[pos] == STRING) {
            std::cout << "Swap strings: " << rp[pos].first << " <=> " << rp[pos].second << std::endl;
            SwapInMatrix(rp[pos].first, rp[pos].second);
        }
        else {
            std::cout << "Swap columns: " << rp[pos].first << " <=> " << rp[pos].second << std::endl;
            SwapInMatrix(rp[pos].first, rp[pos].second, COLUMN);
        }
        if(pos == 0) {
            break;
        }
    }
}

double TMatrix::GetDeterminant() {
    
    double answer = 0;
    
    if(a != b) {
        std::cout << "Error: the determinant of non-square matrix cannot be calculated!" << std::endl;
        std::cout << a << 'x' << b << ':' << std::endl;
        std::cout << *this;
        exit(1);
    }
    else {
        if(a == 1) {
            return m[0][0];
        }
        else if(a == 2) {
            answer = m[0][0] * m[1][1] - m[0][1] * m[1][0];
        }
        else if (a == 3) {
            answer = m[0][0] * m[1][1] * m[2][2]
                   - m[0][0] * m[1][2] * m[2][1]
                   - m[0][1] * m[1][0] * m[2][2]
                   + m[0][1] * m[1][2] * m[2][0]
                   + m[0][2] * m[1][0] * m[2][1]
                   - m[0][2] * m[1][1] * m[2][0];
        }
        else {

            size_t* leadElem = new size_t[a];
            
            for(size_t j = 0; j < b; ++j) {
                leadElem[0] = j;
                answer += pow(-1, j) * m[0][j] * this->recDeterminant(leadElem, 1);
            }

            delete leadElem;
        }
    }

    return answer;
}

size_t TMatrix::GetSA() const {
    return a;
}

size_t TMatrix::GetSB() const {
    return b;
}

size_t TMatrix::GetRp() const {
    return rp.size();
}

bool TMatrix::IsExist() const {
    return (m != nullptr);
}

void TMatrix::Transpose() {
    trans = (trans) ? false : true;
    return;
}

TMatrix TMatrix::T() {
    return TMatrix(*this, (trans ? false : true));
}

void TMatrix::PrintPart(size_t i1, size_t j1, size_t i2, size_t j2) const {
    
    if(m != nullptr) {
        for(size_t i = i1; i <= i2; ++i) {
            for(size_t j = j1; j <= j2; ++j) {
                std::cout << (trans ? m[j][i] : m[i][j]);
                if(j != j2) {
                    std::cout << ", ";
                }
                else {
                    std::cout << std::endl;
                }
            }
        }
    }
    else {
        std::cout << "Empty!" << std::endl;
    }
}

long long TMatrix::recDeterminant(size_t* leadElem, size_t nstr) {

    size_t  sMinor = a - nstr;
    bool isFree = true;

    if(sMinor == 1) {
        for(size_t j = 0; j < b; ++j) {
            for(size_t i = 0; i < nstr; ++i) {
                if(leadElem[i] == j) {
                    isFree = false;
                    break;
                }
            }

            if(isFree) {
                return m[nstr][j];
            }
            else {
                isFree = true;
            }
        }
    }
    else {

        long long   answer = 0;
        size_t      nItem  = 1;
        
        for(size_t j = 0; j < b; ++j) {
            for(size_t i = 0; i < nstr; ++i) {
                if(leadElem[i] == j) {
                    isFree = false;
                    break;
                }
            }

            if(isFree) {
                leadElem[nstr] = j;
                answer += pow(-1, nItem + 1) * m[nstr][j] * this->recDeterminant(leadElem, nstr + 1);
                ++nItem;
            }
            else {
                isFree = true;
            }
        }

        return answer;
    }

    std::cout << "Error: calc determination failed!" << std::endl;
    std::cout << "Info: ";
    for(size_t i = 0; i < a; ++i) {
        std::cout << leadElem[i] << ' ';
    }
    std::cout << std::endl;

    std::cout << "Matrix: " << std::endl;
    std::cout << *this;
    exit(1);
}

std::ostream& operator << (std::ostream &out, const TMatrix &matrix) {

    size_t a = (matrix.trans ? matrix.b : matrix.a);
    size_t b = (matrix.trans ? matrix.a : matrix.b);
    
    
    if(matrix.m != nullptr) {
        for(size_t i = 0; i < a; ++i) {
            for(size_t j = 0; j < b; ++j) {
                out << (matrix.trans ? matrix.m[j][i] : matrix.m[i][j]);
                if(j != b - 1) {
                    out << ", ";
                }
                else {
                    out << std::endl;
                }
            }
        }
    }
    else {
        out << "Empty!" << std::endl;
    }

    return out;
}

bool operator == (const TMatrix &left, const TMatrix &right) {

    size_t a1 = (left.trans)  ? left.b  : left.a;
    size_t b1 = (left.trans)  ? left.a  : left.b;
    size_t a2 = (right.trans) ? right.b : right.a;
    size_t b2 = (right.trans) ? right.a : right.b;

    if(a1 != a2 || b1 != b2) {
        return false;
    }
    else {
        
        for(size_t i = 0; i < a1; ++i) {
            for(size_t j = 0; j < b1; ++j) {
                if((left.trans ? left.m[j][i] : left.m[i][j]) != (right.trans ? right.m[j][i] : right.m[i][j])) {
                    return false;
                }
            }
        }

        return true;
    }
}

bool operator != (const TMatrix &left, const TMatrix &right) {

    size_t a1 = (left.trans)  ? left.b  : left.a;
    size_t b1 = (left.trans)  ? left.a  : left.b;
    size_t a2 = (right.trans) ? right.b : right.a;
    size_t b2 = (right.trans) ? right.a : right.b;

    if(a1 != a2 || b1 != b2) {
        return true;
    }
    else {
        
        for(size_t i = 0; i < a1; ++i) {
            for(size_t j = 0; j < b1; ++j) {
                if((left.trans ? left.m[j][i] : left.m[i][j]) != (right.trans ? right.m[j][i] : right.m[i][j])) {
                    return true;
                }
            }
        }

        return false;
    }
}

const TMatrix operator + (const TMatrix &left, const TMatrix &right) {

    size_t a1 = (left.trans)  ? left.b  : left.a;
    size_t b1 = (left.trans)  ? left.a  : left.b;
    size_t a2 = (right.trans) ? right.b : right.a;
    size_t b2 = (right.trans) ? right.a : right.b;
    
    if(a1 == a2 && b1 == b2) {

        double** m = new double*[a1];
        for(size_t i = 0; i < a1; ++i) {
            m[i] = new double[b1];
        }
        
        for(size_t i = 0; i < a1; ++i) {
            for(size_t j = 0; j < b1; ++j) {
                m[i][j] = (left.trans ? left.m[j][i] : left.m[i][j]) + (right.trans ? right.m[j][i] : right.m[i][j]);
            }
        }

        return TMatrix(a1, b1, m, false);
    }
    else {
        std::cout << "Error: incorrect sum of matrices!" << std::endl;
        std::cout << "Left matrix: " << a1 << "x" << b1 << std::endl;
        std::cout << "Right matrix: " << a2 << "x" << b2 << std::endl;
        exit(1);
    }
}

const TMatrix operator - (const TMatrix &left, const TMatrix &right) {

    size_t a1 = (left.trans)  ? left.b  : left.a;
    size_t b1 = (left.trans)  ? left.a  : left.b;
    size_t a2 = (right.trans) ? right.b : right.a;
    size_t b2 = (right.trans) ? right.a : right.b;
    
    if(a1 == a2 && b1 == b2) {

        double** m = new double*[a1];
        for(size_t i = 0; i < a1; ++i) {
            m[i] = new double[b1];
        }
        
        for(size_t i = 0; i < a1; ++i) {
            for(size_t j = 0; j < b1; ++j) {
                m[i][j] = (left.trans ? left.m[j][i] : left.m[i][j]) - (right.trans ? right.m[j][i] : right.m[i][j]);
            }
        }

        return TMatrix(a1, b1, m, false);
    }
    else {
        std::cout << "Error: incorrect matrix substraction!" << std::endl;
        std::cout << "Left matrix: " << a1 << "x" << b1 << std::endl;
        std::cout << "Right matrix: " << a2 << "x" << b2 << std::endl;
        exit(1);
    }
}

const TMatrix operator * (const TMatrix &left, const TMatrix &right) {

    size_t a  = (left.trans)  ? left.b  : left.a;
    size_t b1 = (left.trans)  ? left.a  : left.b;
    size_t b2 = (right.trans) ? right.b : right.a;
    size_t c  = (right.trans) ? right.a : right.b;
    
    if(b1 == b2) {

        double** m = new double*[a];
        for(size_t i = 0; i < a; ++i) {
            m[i] = new double[c];
        }
        
        for(size_t i = 0; i < a; ++i) {
            for(size_t j = 0; j < c; ++j) {
                m[i][j] = 0;
            }
        }
        
        for(size_t i = 0; i < a; ++i) {
            for(size_t j = 0; j < c; ++j) {
                for(size_t summ = 0; summ < b1; ++summ) {
                    m[i][j] += ((left.trans)  ? left.m[summ][i]  :  left.m[i][summ]) *
                               ((right.trans) ? right.m[j][summ] : right.m[summ][j]);
                }
            }
        }
        
        return TMatrix(a, c, m, false);
    }
    else {
        std::cout << "Error: шncorrect matrix multiplication" << std::endl;
        std::cout << "Left matrix: " << a << "x" << b1 << std::endl;
        std::cout << "Right matrix: " << b2 << "x" << c << std::endl;
        exit(1);
    }
}

const TMatrix operator * (const double &left, const TMatrix &right) {
    
    size_t sa = right.a;
    size_t sb = right.b;
    
    double** m = new double*[sa];
    for(size_t i = 0; i < sa; ++i) {
        m[i] = new double[sb];
    }
    
    for(size_t i = 0; i < sa; ++i) {
        for(size_t j = 0; j < sb; ++j) {
            m[i][j] = right.m[i][j] * left;
        }
    }
    
    return TMatrix(sa, sb, m, false);
}

const TMatrix operator * (const TMatrix &left, const double &right) {

    size_t sa = left.a;
    size_t sb = left.b;
    
    double** m = new double*[sa];
    for(size_t i = 0; i < sa; ++i) {
        m[i] = new double[sb];
    }
    
    for(size_t i = 0; i < sa; ++i) {
        for(size_t j = 0; j < sb; ++j) {
            m[i][j] = left.m[i][j] * right;
        }
    }
    
    return TMatrix(sa, sb, m, false);
}

const TMatrix operator / (const TMatrix &left, const double &right) {

    size_t sa = left.a;
    size_t sb = left.b;
    
    double** m = new double*[sa];
    for(size_t i = 0; i < sa; ++i) {
        m[i] = new double[sb];
    }
    
    for(size_t i = 0; i < sa; ++i) {
        for(size_t j = 0; j < sb; ++j) {
            m[i][j] = left.m[i][j] / right;
        }
    }
    
    return TMatrix(sa, sb, m, false);
}

TMatrix& TMatrix::operator += (const TMatrix &right) {

    size_t a1 = (trans)       ? b : a;
    size_t b1 = (trans)       ? a : b;
    size_t a2 = (right.trans) ? right.b : right.a;
    size_t b2 = (right.trans) ? right.a : right.b;
    
    if(a1 == a2 && b1 == b2) {

        double** m = new double*[a1];
        for(size_t i = 0; i < a1; ++i) {
            m[i] = new double[b1];
        }
        
        for(size_t i = 0; i < a1; ++i) {
            for(size_t j = 0; j < b1; ++j) {
                (trans ? m[j][i] : m[i][j]) += (right.trans ? right.m[j][i] : right.m[i][j]);
            }
        }

        return *this;
    }
    else {
        std::cout << "Error: incorrect sum of matrices!" << std::endl;
        std::cout << "Left matrix: " << a1 << "x" << b1 << std::endl;
        std::cout << "Right matrix: " << a2 << "x" << b2 << std::endl;
        exit(1);
    }
}

TMatrix& TMatrix::operator -= (const TMatrix &right) {
        
    size_t a1 = (trans)       ? b : a;
    size_t b1 = (trans)       ? a : b;
    size_t a2 = (right.trans) ? right.b : right.a;
    size_t b2 = (right.trans) ? right.a : right.b;
    
    if(a1 == a2 && b1 == b2) {

        double** m = new double*[a1];
        for(size_t i = 0; i < a1; ++i) {
            m[i] = new double[b1];
        }
        
        for(size_t i = 0; i < a1; ++i) {
            for(size_t j = 0; j < b1; ++j) {
                (trans ? m[j][i] : m[i][j]) -= (right.trans ? right.m[j][i] : right.m[i][j]);
            }
        }

        return *this;
    }
    else {
        std::cout << "Error: incorrect matrix substraction!" << std::endl;
        std::cout << "Left matrix: " << a1 << "x" << b1 << std::endl;
        std::cout << "Right matrix: " << a2 << "x" << b2 << std::endl;
        exit(1);
    }
}

TMatrix& TMatrix::operator *= (const TMatrix &right) {

    if(m == nullptr) {
        std::cout << "Error: шncorrect matrix multiplication" << std::endl;
        std::cout << "Get nullptr of left operand" << std::endl;
        exit(1);
    }

    if(right.m == nullptr) {
        std::cout << "Error: шncorrect matrix multiplication" << std::endl;
        std::cout << "Get nullptr of right operand" << std::endl;
        exit(1);
    }
    
    size_t na  =       (trans) ?       b :       a;
    size_t b1  =       (trans) ?       a :       b;
    size_t b2  = (right.trans) ? right.b : right.a;
    size_t nc  = (right.trans) ? right.a : right.b;
    
    if(b1 == b2) {

        double** tmp = new double*[na];
        for(size_t i = 0; i < na; ++i) {
            tmp[i] = new double[nc];
        }
        
        for(size_t i = 0; i < na; ++i) {
            for(size_t j = 0; j < nc; ++j) {
                tmp[i][j] = 0;
            }
        }
        
        for(size_t i = 0; i < na; ++i) {
            for(size_t j = 0; j < nc; ++j) {
                for(size_t summ = 0; summ < b1; ++summ) {
                    tmp[i][j] += ((trans)       ?       m[summ][i] :       m[i][summ]) *
                                 ((right.trans) ? right.m[j][summ] : right.m[summ][j]);
                }
            }
        }

        for(size_t i = 0; i < a; ++i) {
                delete m[i];
        }
        delete m;

        m = tmp;
        a = na;
        b = nc;

        rp.clear();
        trp.clear();

        trans = false;
        return *this;
    }
    else {
        std::cout << "Error: шncorrect matrix multiplication" << std::endl;
        std::cout << "Left matrix: " << na << "x" << b1 << std::endl;
        std::cout << "Right matrix: " << b2 << "x" << nc << std::endl;
        exit(1);
    }
}

TMatrix& TMatrix::operator *= (const double &right) {
    
    for(size_t i = 0; i < a; ++i) {
        for(size_t j = 0; j < b; ++j) {
            m[i][j] *= right;
        }
    }
    
    return *this;
}

TMatrix& TMatrix::operator /= (const double &right) {
    
    for(size_t i = 0; i < a; ++i) {
        for(size_t j = 0; j < b; ++j) {
            m[i][j] /= right;
        }
    }
    
    return *this;
}

TMatrix& TMatrix::operator = (const TMatrix& right) {

    if(this == &right) {
        return *this;
    }

    if(m != nullptr) {
        for(size_t i = 0; i < a; ++i) {
            delete m[i];
        }
        delete m;
    }
    
    a = right.a;
    b = right.b;

    trp = right.trp;
    rp = right.rp;
    
    trans = right.trans;
    own = true;

    m = new double* [a];
    for(size_t i = 0; i < a; ++i) {
        m[i] = new double[b];
    }

    for(size_t i = 0; i < a; ++i) {
        for(size_t j = 0; j < b; ++j) {
            m[i][j] = right.m[i][j];
        }
    }

    return *this;
}

double* TMatrix::operator [] (const size_t index) const {
    return m[index];
}
        

std::pair<TMatrix, TMatrix> LUDecomposition(TMatrix& A, TMatrix B, bool checkLU) {

    size_t sizeM = A.GetSA();
    double coef;

    TMatrix X(sizeM, sizeM, ZERO_MATRIX);
    TMatrix L(sizeM, sizeM, UNIT_MATRIX);
    TMatrix U(A);
    
    for(size_t k = 0; k < sizeM - 1; ++k) {
        for(size_t i = k + 1; i < sizeM; ++i) {
            
            if(U[k][k] == 0) {
                for(size_t s = k + 1; s < sizeM; ++s) {
                    if(U[s][k] != 0) {
                        U.SwapInMatrix(s, k); //После него проверка будет выдавать отрицательный результат
                        B.SwapInMatrix(s, k);
                        break;
                    }
                }
            }

            coef = U[i][k] / U[k][k];
            
            for(size_t j = k; j < sizeM; ++j) {
                U[i][j] = U[i][j] - coef * U[k][j];
            }

            size_t sizeB = B.GetSB();
            for(size_t bj = 0; bj < sizeB; ++bj) {
                B[i][bj] = B[i][bj] - coef * B[k][bj];
            }
        
            L[i][k] = coef;
        }
    }

    if(checkLU) {
        std::cout << "L: " << std::endl;
        std::cout << L;
        std::cout << "U: " << std::endl;
        std::cout << U;
        std::cout << "L*U = A? - ";
        if(A != L*U) {
            std::cout << "false" << std::endl;
            std::cout << "Check:" << std::endl;
            std::cout << L*U;
            std::cout << "Original: " << std::endl;
            std::cout << A;
            std::cout << "Try to swap strings..." << std::endl;
    
            TMatrix T(U);
            T.SwapBackward(BACK);
            std::cout << "L*U = A? - ";
            if(A == (L*T)) {
                std::cout << "true" << std::endl;
            }
            else {
                std::cout << "false" << std::endl;
                std::cout << "LU-Decomposition failed!" << std::endl;
                exit(1);
            }
            
        }
        else {
            std::cout << "true" << std::endl;
            std::cout << "Check:" << std::endl;
            std::cout << L*U;
            std::cout << "Original: " << std::endl;
            std::cout << A;
        }
    }
    
    for(size_t k = 0; k < sizeM - 1; ++k) {
        for(size_t i = k + 1; i < sizeM; ++i) {
            U[i][k] = L[i][k];
        }
    }
    
    return std::pair<TMatrix, TMatrix>(U, B);
}

TMatrix InverseMatrix(TMatrix& A) {

    size_t sizeM = A.GetSA();
    double coef;
    std::vector<std::pair<size_t, size_t>> perm;

    TMatrix U(A);
    TMatrix X(sizeM, sizeM, ZERO_MATRIX);
    TMatrix L(sizeM, sizeM, UNIT_MATRIX);
    TMatrix B(sizeM, sizeM, UNIT_MATRIX);

    for(size_t k = 0; k < sizeM - 1; ++k) {
        for(size_t i = k + 1; i < sizeM; ++i) {
            
            if(U[k][k] == 0) {
                for(size_t s = k + 1; s < sizeM; ++s) {
                    if(U[s][k] != 0) {
                        U.SwapInMatrix(s, k); //После него проверка будет выдавать отрицательный результат
                        B.SwapInMatrix(s, k);
                        perm.push_back(std::pair<size_t, size_t>(s, k));
                        break;
                    }
                }
            }

            coef = U[i][k] / U[k][k];
            
            for(size_t j = k; j < sizeM; ++j) {
                U[i][j] = U[i][j] - coef * U[k][j];
            }

            size_t sizeB = B.GetSB();
            for(size_t bj = 0; bj < sizeB; ++bj) {
                B[i][bj] = B[i][bj] - coef * B[k][bj];
            }
            
            L[i][k] = coef;
        }
    }
    
    for(size_t i = 0; i < sizeM; ++i) {
        for(size_t k = sizeM - 1; k >= 0; --k) {
            for(size_t j = sizeM - 1; j >= k; --j) {
    
                if(j == k) {
                    B[k][i] /= U[k][j];
                }
                else {
                    B[k][i] -= U[k][j] * X[j][i];
                }
                
                if(j == 0) {
                    break;
                }
            }
            
            X[k][i] = B[k][i];
    
            if(k == 0) {
                break;
            }
        }
    }
    
    return X;
}

TMatrix SolveSystemViaLU(TMatrix& LU, TMatrix B) {

    size_t sizeM = LU.GetSA();
    TMatrix X(sizeM, 1, ZERO_MATRIX);
    
    for(size_t k = sizeM - 1; k >= 0; --k) {

        for(size_t j = sizeM - 1; j >= k; --j) {

            if(j == k) {
                B[k][0] = B[k][0] / LU[k][j];
            }
            else {
                B[k][0] = B[k][0] - LU[k][j] * X[j][0];
            }
            
            if(j == 0) {
                break;
            }
        }
        
        X[k][0] = B[k][0];

        if(k == 0) {
            break;
        }
    }

    return X;
}

double GetDeterminant(TMatrix& LU) {
    size_t sizeM = LU.GetSA();
    double answer = pow(-1, LU.GetRp());
    for(size_t k = 0; k < sizeM; ++k) {
        answer *= LU[k][k];
    }
    return answer;
}

TMatrix SweepMethod(TMatrix& A, TMatrix& B) {

    size_t sizeM = A.GetSA();
    size_t li = sizeM - 1;
    double* P = new double[sizeM];
    double* Q = new double[sizeM];

    if(!B.IsExist()) {
        B = TMatrix(sizeM, 1, ZERO_MATRIX);
    }
    
    P[0] = -(A[0][1] / A[0][0]);
    Q[0] =   B[0][0] / A[0][0];
    for(size_t i = 1; i < li; ++i) {
        P[i] = - (A[i][i + 1]                     / (A[i][i] + A[i][i - 1]*P[i - 1]));
        Q[i] =   (B[i][0] - A[i][i - 1]*Q[i - 1]) / (A[i][i] + A[i][i - 1]*P[i - 1]);
    }

    P[li] = 0;
    Q[li] = (B[li][0] - A[li][li - 1]*Q[li - 1]) / (A[li][li] + A[li][li - 1]*P[li - 1]);

    TMatrix X(sizeM, 1, ZERO_MATRIX);

    X[li][0] = Q[li];
    for(size_t i = li - 1; i >= 0; --i) {

        X[i][0] = P[i]*X[i + 1][0] + Q[i];

        if(i == 0) {
            break;
        }
    }

    return X;
}

TMatrix SimpleIterations(TMatrix A, TMatrix B) {

    double eps = 0.001;
    size_t sizeM = A.GetSA();

    if(!B.IsExist()) {
        B = TMatrix(sizeM, 1, ZERO_MATRIX);
    }

    for(size_t i = 0; i < sizeM; ++i) {

        double diag = A[i][i];

        B[i][0] /= diag;

        for(size_t j = 0; j < sizeM; ++j) {
            A[i][j] = (i == j) ? 0 : (- (A[i][j] / diag));
        }
    }

    TMatrix X(sizeM, 1, ZERO_MATRIX);
    TMatrix PrevX(sizeM, 1, ZERO_MATRIX);
    TMatrix Delta(sizeM, 1, ZERO_MATRIX);
    PrevX = B;
    X = B + A * B;
    Delta = X - PrevX;
    for(size_t iter = 1; Delta.CalcNorm() > eps; ++iter) {
        PrevX = X;
        X = B + A * X;
        Delta = X - PrevX;
        if(Delta.CalcNorm() < eps) {
            std::cout << "Iterations = " << iter << std::endl;
        }
    }

    return X;
}

TMatrix SeidelMethod(TMatrix A, TMatrix B) {

    double eps = 0.001;
    size_t sizeM = A.GetSA();

    if(!B.IsExist()) {
        B = TMatrix(sizeM, 1, ZERO_MATRIX);
    }

    for(size_t i = 0; i < sizeM; ++i) {

        double diag = A[i][i];

        B[i][0] /= diag;

        for(size_t j = 0; j < sizeM; ++j) {
            A[i][j] = (i == j) ? 0 : (- (A[i][j] / diag));
        }
    }

    TMatrix X(sizeM, 1, ZERO_MATRIX);
    TMatrix PrevX(sizeM, 1, ZERO_MATRIX);
    TMatrix Delta(sizeM, 1, ZERO_MATRIX);
    PrevX = B;

    for(size_t i = 0; i < sizeM; ++i) {
        X[i][0] = B[i][0];
        for(size_t j = 0; j < sizeM; ++j) {
            X[i][0] += (j < i) ? (A[i][j]*X[j][0]) : (A[i][j]*PrevX[j][0]);
        }
    }
    
    X = B + A * B;
    Delta = X - PrevX;
    for(size_t iter = 1; Delta.CalcNorm() > eps; ++iter) {

        for(size_t i = 0; i < sizeM; ++i) {
            X[i][0] = B[i][0];
            for(size_t j = 0; j < sizeM; ++j) {
                X[i][0] += (j < i) ? (A[i][j]*X[j][0]) : (A[i][j]*PrevX[j][0]);
            }
        }
        
        Delta = X - PrevX;
        PrevX = X;
        
        if(Delta.CalcNorm() < eps) {
            std::cout << "Iterations = " << iter << std::endl;
        }
    }

    return X;
}

std::pair<TMatrix, TMatrix> RotationMethod(TMatrix A, double eps) {

    size_t sizeM = A.GetSA();
    size_t mi, mj;

    double fi, norm = 2 * eps;
    
    TMatrix U(sizeM, sizeM, UNIT_MATRIX);
    TMatrix E(sizeM, sizeM, UNIT_MATRIX);
    TMatrix nextU;
    
    for(size_t iter = 0; norm > eps; ++iter) {

        mi = 0;
        mj = 1;
        
        for(size_t i = 0; i < sizeM; ++i) {
            for(size_t j = i + 1; j < sizeM; ++j) {
                if((A[i][j] < 0 ? -A[i][j]: A[i][j]) > (A[mi][mj] < 0 ? -A[mi][mj] : A[mi][mj])) {
                    mi = i;
                    mj = j;
                }
            }
        }
        
        
        if(A[mi][mi] == A[mj][mj]) {
            fi = M_PI / 4;
        }
        else {
            fi = 0.5 * atan( (2*A[mi][mj]) / (A[mi][mi] - A[mj][mj]) );
        }

        nextU = E;
        nextU[mi][mi] = cos(fi);
        nextU[mi][mj] = -sin(fi);
        nextU[mj][mi] = sin(fi);
        nextU[mj][mj] = cos(fi);
        
        U *= nextU;
        
        A = A * nextU;
        
        A = nextU.T() * A;

        norm = 0;
        for(size_t i = 0; i < sizeM; ++i) {
            for(size_t j = i + 1; j < sizeM; ++j) {
                norm += A[i][j]*A[i][j];
            }
        }
        norm = sqrt(norm);
        
    }
    
    return std::pair<TMatrix, TMatrix>(A, U);
}

std::pair<TMatrix, TMatrix> QRAlgorithm(TMatrix A, double eps) {

    size_t sizeM = A.GetSA();
    TMatrix V, L, H;
    TMatrix E(sizeM, sizeM, UNIT_MATRIX);
    TMatrix Check = A;
    TMatrix Q = E;
    double summ;
    for(size_t iter = 0; iter < sizeM; ++iter) {
        for(size_t k = 0; k < sizeM; ++k) {
            V = TMatrix(A, 0, k, sizeM - 1, k);
            for(size_t i = 0; i < k; ++i) {
                V[i][0] = 0;
            }
            summ = 0;
            for(size_t i = 0; i < sizeM; ++i) {
                summ += A[i][k]*A[i][k];
            }
            summ = sqrt(summ);
            
            V[k][0] = A[k][k] + ((A[k][k] >= 0) ? 1 : -1)*summ;
            
            L = V.T()*V;
            H = E - (2/L[0][0])*(V*V.T());
            A = H*A;
            Q *= H;
        }
    }
    return std::pair<TMatrix, TMatrix>(Q,A);
}
