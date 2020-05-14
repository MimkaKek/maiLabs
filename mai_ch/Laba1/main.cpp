#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>

enum MTypes {
    ZERO_MATRIX,
    UNIT_MATRIX
};

enum ConvTypes {
    STRING,
    COLUMN
};

enum LUDecomp {
    NORMAL,
    REVERSE
};

enum NormTypes {
    TYPE_1,
    TYPE_2,
    TYPE_C
};

class TMatrix {
    public:

        TMatrix(): trans(false), ownM(true), bi(0), bj(0), ei(0), ej(0), li(0), lj(0), rp(0), m(nullptr) {}

        TMatrix(const TMatrix &orig) {


            rp    = orig.rp;
            trans = orig.trans;

            bi = 0;
            bj = 0;

            ei = orig.ei - orig.bi;
            ej = orig.ej - orig.bj;

            li = orig.li;
            lj = orig.lj;
            
            ownM = true;
            
            m = new double*[li];
            for(size_t i = 0; i < li; ++i) {
                m[i] = new double[lj];
            }

            for(size_t soi = orig.bi, si = bi; si <= ei; ++soi, ++si) {
                for(size_t soj = orig.bj, sj = bj; sj <= ej; ++soj, ++sj) {
                    m[si][sj] = orig.m[soi][soj];
                }
            }
        }

        TMatrix(const TMatrix &orig, bool t) {


            rp    = orig.rp;
            trans = t;

            bi = 0;
            bj = 0;

            ei = orig.ei - orig.bi;
            ej = orig.ej - orig.bj;

            li = orig.li;
            lj = orig.lj;
            
            ownM = true;
            
            m = new double*[li];
            for(size_t i = 0; i < li; ++i) {
                m[i] = new double[lj];
            }

            for(size_t soi = orig.bi, si = bi; si <= ei; ++soi, ++si) {
                for(size_t soj = orig.bj, sj = bj; sj <= ej; ++soj, ++sj) {
                    m[si][sj] = orig.m[soi][soj];
                }
            }
        }

        TMatrix(const TMatrix &orig, size_t i1, size_t j1, size_t i2, size_t j2, bool copy = true) {

            li = i2 - i1 + 1;
            lj = j2 - j1 + 1;
            
            rp    = 0;
            trans = 0;
            
            if(copy) {

                bi = 0;
                bj = 0;

                ei = li - 1;
                ej = lj - 1;
                
                ownM = true;
                
                m = new double*[li];
                for(size_t i = 0; i < li; ++i) {
                    m[i] = new double[lj];
                }
    
                for(size_t soi = i1, si = bi; soi <= i2; ++soi, ++si) {
                    for(size_t soj = j1, sj = bj; soj <= j2; ++soj, ++sj) {
                        m[si][sj] = orig.m[soi][soj];
                    }
                }
                
            }
            else {

                bi = i1;
                bj = j1;

                ei = i2;
                ej = j2;

                ownM = false;
                
                m = orig.m;
            }
        }
        
        TMatrix(std::istream &is) {

            std::cout << "Enter the number of lines: ";
            is >> li;
            std::cout << "Enter the number of columns: ";
            is >> lj;
            
            rp = 0;

            bi = 0;
            bj = 0;

            ei = li - 1;
            ej = lj - 1;
            
            ownM = true;
            
            trans = false;
            
            m = new double*[li];
            for(size_t i = bi; i < li; ++i) {
                m[i] = new double[lj];
            }
            
            std::cout << "Enter items of matrix:" << std::endl;
            for(size_t i = bi; i <= ei; ++i) {
                for(size_t j = bj; j <= ej; ++j) {
                    is >> m[i][j];
                }
            }
        }
        
        TMatrix(size_t i, size_t j, double **matrix, bool copy = true) {

            li = i;
            lj = j;

            rp = 0;
            

            bi = 0;
            bj = 0;

            ei = li - 1;
            ej = lj - 1;

            ownM = true;
            trans = false;
            if(copy) {
                m = new double*[li];
                for(size_t i = bi; i < li; ++i) {
                    m[i] = new double[lj];
                }
    
                if(matrix == nullptr) {
                    for(size_t si = bi; si <= ei; ++si) {
                        for(size_t sj = bj; sj <= ej; ++sj) {
                            m[si][sj] = 0;
                        }
                    }
                }
                else {
                    for(size_t si = bi; si <= ei; ++si) {
                        for(size_t sj = bj; sj <= ej; ++sj) {
                            m[si][sj] = matrix[si][sj];
                        }
                    }
                }
            }
            else {
                m = matrix;
            }
            
        };

        TMatrix(size_t i, size_t j, size_t type = ZERO_MATRIX) {

            li = i;
            lj = j;

            rp = 0;

            bi = 0;
            bj = 0;
            
            ei = li - 1;
            ej = lj - 1;
            
            ownM  = true;
            trans = false;
            
            m = new double*[li];
            for(size_t i = bi; i < li; ++i) {
                m[i] = new double[lj];
            }

            switch(type) {
                case ZERO_MATRIX:
                    for(size_t si = bi; si <= ei; ++si) {
                        for(size_t sj = bj; sj <= ej; ++sj) {
                            m[si][sj] = 0;
                        }
                    }
                    break;
                case UNIT_MATRIX:
                    for(size_t si = bi; si <= ei; ++si) {
                        for(size_t sj = bj; sj <= ej; ++sj) {
                            m[si][sj] = (si == sj ? 1 : 0);
                        }
                    }
                    break;
                default:
                    for(size_t si = bi; si <= ei; ++si) {
                        for(size_t sj = bj; sj <= ej; ++sj) {
                            m[si][sj] = 0;
                        }
                    }
                    break;
            }
        };
        
        ~TMatrix() {
            if(m != nullptr && ownM) {
                for(size_t si = bi; si < li; ++si) {
                    delete m[si];
                }
                delete m;
            }
        }

        double CalcNorm(unsigned char type = TYPE_2) {
            if(li == 1) {
                switch(type) {
                    case TYPE_1:
                    {
                        double norm = 0;
                        for(size_t sj = bj; sj <= ej; ++sj) {
                            norm += (m[bi][sj] < 0) ? -m[bi][sj] : m[bi][sj];
                        }
                        return norm;
                    }
                    case TYPE_2:
                    {
                        double norm = 0;
                        for(size_t sj = bj; sj <= ej; ++sj) {
                            norm += m[bi][sj] * m[bi][sj];
                        }

                        norm = sqrt(norm);
                        
                        return norm;
                    }
                    case TYPE_C:
                    {
                        double norm = m[bi][bj];
                        for(size_t sj = bj; sj <= ej; ++sj) {
                            norm = (norm < m[bi][sj]) ? m[bi][sj] : norm;
                        }
                        return norm;
                    }
                    default:
                        std::cout << "Error: Can't calculate this type of norm of vector!" << std::endl;
                        exit(1);
                }
            }
            else if(lj == 1) {
                switch(type) {
                    case TYPE_1:
                    {
                        double norm = 0;
                        for(size_t si = bi; si <= ei; ++si) {
                            norm += (m[si][bj] < 0) ? -m[si][bj] : m[si][bj];
                        }
                        return norm;
                    }
                    case TYPE_2:
                    {
                        double norm = 0;
                        for(size_t si = bi; si <= ei; ++si) {
                            norm += m[si][bj] * m[si][bj];
                        }

                        norm = sqrt(norm);
                        
                        return norm;
                    }
                    case TYPE_C:
                    {
                        double norm = m[bi][bj];
                        for(size_t si = bi; si <= ei; ++si) {
                            norm = (norm < m[si][bj]) ? m[si][bj] : norm;
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

                        for(size_t si = bi; si <= ei; ++si) {
                            norm += (m[si][bj] < 0) ? -m[si][bj] : m[si][bj];
                        }

                        for(size_t sj = bj + 1; sj <= ej; ++sj) {

                            nextNorm = 0;
                            for(size_t si = bi; si <= ei; ++si) {
                                nextNorm += (m[si][sj] < 0) ? -m[si][sj] : m[si][sj];
                            }

                            norm = (norm < nextNorm) ? nextNorm : norm;
                        }
                        
                        return norm;
                    }
                    case TYPE_2:
                    {
                        double norm = 0;
                        for(size_t si = bi; si <= ei; ++si) {
                            for(size_t sj = bj + 1; sj <= ej; ++sj) {
                                norm += m[si][sj] * m[si][sj];
                            }
                        }

                        norm = sqrt(norm);
                        
                        return norm;
                    }
                    case TYPE_C:
                    {
                        double norm = 0;
                        double nextNorm;

                        for(size_t sj = bj + 1; sj <= ej; ++sj) {
                            norm += (m[bi][sj] < 0) ? -m[bi][sj] : m[bi][sj];
                        }

                        for(size_t si = bi + 1; si <= ei; ++si) {

                            nextNorm = 0;
                            for(size_t sj = bj; sj <= ej; ++sj) {
                                nextNorm += (m[si][sj] < 0) ? -m[si][sj] : m[si][sj];
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
        
        void SwapInMatrix(size_t i1, size_t i2, unsigned char type = STRING) {

            double tmp;

            CreateOwnTable();
            
            if(type == STRING) {
                for(size_t sj = bj; sj <= ej; ++sj) {
                    tmp = m[i1][sj];
                    m[i1][sj] = m[i2][sj];
                    m[i2][sj] = tmp;
                }   
            }
            else {
                for(size_t si = bi; si <= ei; ++si) {
                    tmp = m[si][i1];
                    m[si][i1] = m[si][i2];
                    m[si][i2] = tmp;
                }
            }
            ++rp;
            return;
        }
        
        double GetDeterminant() {
            
            double answer = 0;
            
            if(li != lj) {
                std::cout << "Error: the determinant of non-square matrix cannot be calculated!" << std::endl;
                std::cout << li << 'x' << lj << ':' << std::endl;
                std::cout << *this;
                exit(1);
            }
            else {
                if(li == 1) {
                    return m[bi][bj];
                }
                else if(li == 2) {
                    answer = m[bi][bj]     * m[bi + 1][bj + 1]
                           - m[bi][bj + 1] * m[bi + 1][bj];
                }
                else if (li == 3) {
                    answer = m[bi][bj]     * m[bi + 1][bj + 1] * m[bi + 2][bj + 2]
                           - m[bi][bj]     * m[bi + 1][bj + 2] * m[bi + 2][bj + 1]
                           - m[bi][bj + 1] * m[bi + 1][bj]     * m[bi + 2][bj + 2]
                           + m[bi][bj + 1] * m[bi + 1][bj + 2] * m[bi + 2][bj]
                           + m[bi][bj + 2] * m[bi + 1][bj]     * m[bi + 2][bj + 1]
                           - m[bi][bj + 2] * m[bi + 1][bj + 1] * m[bi + 2][bj];
                }
                else {

                    size_t* leadElem = new size_t[li];
                    
                    for(size_t sj = 0; sj <= ej; ++sj) {
                        leadElem[bi] = sj;
                        answer += pow(-1, sj) * m[bi][sj] * this->recDeterminant(leadElem, bi + 1);
                    }

                    delete leadElem;
                }
            }

            return answer;
        }

        size_t GetSA() {
            return li;
        }

        size_t GetSB() {
            return lj;
        }
        
        size_t GetRp() {
            return rp;
        }

        bool IsExist() {
            return (m != nullptr);
        }

        void Transpose() {
            trans = (trans) ? false : true;
            return;
        }

        TMatrix T() {
            return TMatrix(*this, (trans ? false : true));
        }
        
        friend const TMatrix operator + (const TMatrix &left, const TMatrix &right);
        friend const TMatrix operator - (const TMatrix &left, const TMatrix &right);
        friend const TMatrix operator * (const TMatrix &left, const TMatrix &right);
        friend const TMatrix operator * (const double &left, const TMatrix &right);
        friend const TMatrix operator * (const TMatrix &left, const double &right);
        friend bool operator == (const TMatrix &left, const TMatrix &right);
        friend bool operator != (const TMatrix &left, const TMatrix &right);

        friend std::ostream& operator << (std::ostream &out, const TMatrix &matrix);
        
        TMatrix operator += (const TMatrix &right) {
            if(li == right.li && lj == right.lj) {

                if(!ownM) {
                    CreateOwnTable();
                }
                
                if(li != lj) {
                    if(trans != right.trans) {
                        std::cout << "Error: incorrect sum of matrices" << std::endl;
                        std::cout << "Left matrix: " << li << "x" << lj << " | " << trans << std::endl;
                        std::cout << "Right matrix: " << right.li << "x" << right.lj << " | " << right.trans << std::endl;
                        exit(1);
                    }
                }

                for(size_t si = bi, soi = right.bi; si <= ei; ++si, ++soi) {
                    for(size_t sj = bj, soj = right.bj; sj <= ej; ++sj, ++soj) {
                        (trans ? m[sj][si] : m[si][sj]) +=  (right.trans ? right.m[soj][soi] : right.m[soi][soj]);
                    }
                }
        
                return *this;
            }
            else {
                std::cout << "Error: incorrect sum of matrices" << std::endl;
                std::cout << "Left matrix: " << li << "x" << lj << " | " << trans << std::endl;
                std::cout << "Right matrix: " << right.li << "x" << right.lj << " | " << right.trans << std::endl;
                exit(1);
            }
        };
        
        TMatrix& operator -= (const TMatrix &right) {
            if(li == right.li && lj == right.lj) {

                if(!ownM) {
                    CreateOwnTable();
                }
                
                if(li != lj) {
                    if(trans != right.trans) {
                        std::cout << "Error: incorrect sum of matrices" << std::endl;
                        std::cout << "Left matrix: " << li << "x" << lj << " | " << trans << std::endl;
                        std::cout << "Right matrix: " << right.li << "x" << right.lj << " | " << right.trans << std::endl;
                        exit(1);
                    }
                }
                
                for(size_t si = bi, soi = right.bi; si <= ei; ++si, ++soi) {
                    for(size_t sj = bj, soj = right.bj; sj <= ej; ++sj, ++soj) {
                        (trans ? m[sj][si] : m[si][sj]) -=  (right.trans ? right.m[soj][soi] : right.m[soi][soj]);
                    }
                }
        
                return *this;
            }
            else {
                std::cout << "Error: incorrect sum of matrices" << std::endl;
                std::cout << "Left matrix: " << li << "x" << lj << " | " << trans << std::endl;
                std::cout << "Right matrix: " << right.li << "x" << right.lj << " | " << right.trans << std::endl;
                exit(1);
            }
        };
        
        TMatrix& operator *= (const TMatrix &right) {
            
            size_t a  = (trans) ? lj : li;
            size_t b1 = (trans) ? li : lj;
            size_t b2 = (right.trans) ? right.lj : right.li;
            size_t c  = (right.trans) ? right.li : right.lj;
            
            if(b1 == b2) {
        
                double** tmp = new double*[a];
                for(size_t si = 0; si < a; ++si) {
                    tmp[si] = new double[c];
                }
                
                for(size_t si = 0; si < a; ++si) {
                    for(size_t sj = 0; sj < c; ++sj) {
                        tmp[si][sj] = 0;
                    }
                }

                size_t nbi = 0;
                size_t nbj = 0;
                size_t nei = a - 1;
                size_t nej = c - 1;
                
                for(size_t si = nbi, lsi = bi; si <= nei; ++si, ++lsi) {
                    for(size_t sj = nbj, rsj = right.bj; sj <= nej; ++sj, ++rsj) {
                        for(size_t summ = 0, lsj = bj, rsi = right.bi; summ < b1; ++summ, ++lsj, ++rsi) {
                            tmp[si][sj] += ((trans)       ?       m[lsj][lsi] :       m[lsi][lsj]) *
                                           ((right.trans) ? right.m[rsj][rsi] : right.m[rsi][rsj]);
                        }
                    }
                }

                if(ownM) {
                    for(size_t si = bi; si < li; ++si) {
                        delete m[si];
                    }
                    delete m;
                }

                bi = nbi;
                bj = nbj;
                ei = nei;
                ej = nej;
                li = a;
                lj = c;

                trans = false;
                
                m = tmp;
                
                return *this;
            }
            else {
                std::cout << "Error: incorrect matrix subtraction" << std::endl;
                std::cout << "Left matrix: " << a << "x" << b1 << " | " << trans << std::endl;
                std::cout << "Right matrix: " << b2 << "x" << c << " | " << right.trans << std::endl;
                exit(1);
            }
        };

        TMatrix& operator = (const TMatrix& right) {

            if(this == &right) {
                return *this;
            }

            if(m != nullptr && ownM) {
                for(size_t si = bi; si <= ei; ++si) {
                    delete m[si];
                }
                delete m;
            }
            
            bi = 0;
            bj = 0;

            li = right.li;
            lj = right.lj;

            ei = li - 1;
            ej = lj - 1;
            
            rp = right.rp;
            
            trans = right.trans;

            m = new double*[li];
            for(size_t si = 0; si < li; ++si) {
                m[si] = new double[lj];
            }
            
            for(size_t si = bi, soi = right.bi; si <= ei; ++si, ++soi) {
                for(size_t sj = bi, soj = right.bj; sj <= ej; ++sj, ++soj) {
                    m[si][sj] = right.m[soi][soj];
                }
            }

            return *this;
        }

        double* operator [] (const size_t index) {
            return m[index];
        }
        
    private:

        bool            trans;
        bool            ownM;

        size_t          bi;
        size_t          bj;
        size_t          ei;
        size_t          ej;
        size_t          li;
        size_t          lj;
        
        size_t          rp;

        double**        m;

        long long recDeterminant(size_t* leadElem, size_t nstr) {
            size_t  sMinor = li - nstr;
            bool isFree = true;

            if(sMinor == 1) {
                for(size_t j = 0; j < lj; ++j) {
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
                size_t      nItem = 1;
                for(size_t j = 0; j < lj; ++j) {
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
            for(size_t i = 0; i < li; ++i) {
                std::cout << leadElem[i] << ' ';
            }
            std::cout << std::endl;

            std::cout << "Matrix: " << std::endl;
            std::cout << *this;
            exit(1);
        }

        void CreateOwnTable() {
            if(!ownM) {

                size_t obi = bi;
                size_t obj = bj;

                bi = 0;
                bj = 0;

                ei = li - 1;
                ej = lj - 1;
                
                ownM = true;
                
                double** toCopy = m;
                
                m = new double*[li];
                for(size_t si = bi; si < li; ++si) {
                    m[si] = new double[lj];
                }

                for(size_t soi = obi, si = bi; si <= ei; ++si, ++soi) {
                    for(size_t soj = obj, sj = bj; sj <= ej; ++sj, ++soj) {
                        m[si][sj] = toCopy[soi][soj];
                    }
                }
            }

            return;
        }
};

std::ostream& operator << (std::ostream &out, const TMatrix &matrix) {

    if(!matrix.trans) {
        if(matrix.m != nullptr) {
            for(size_t si = matrix.bi; si <= matrix.ei; ++si) {
                for(size_t sj = matrix.bj; sj <= matrix.ej; ++sj) {
                    out << matrix.m[si][sj];
                    if(sj != matrix.ej) {
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
    }
    else {
        if(matrix.m != nullptr) {
            for(size_t sj = matrix.bj; sj <= matrix.ej; ++sj) {
                for(size_t si = matrix.bi; si <= matrix.ei; ++si) {
                    out << matrix.m[si][sj];
                    if(si != matrix.ei) {
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
    }

    return out;
}

bool operator == (const TMatrix &left, const TMatrix &right) {

    size_t a1  = (left.trans) ? left.lj : left.li;
    size_t b1 = (left.trans) ? left.li : left.lj;
    size_t a2 = (right.trans) ? right.lj : right.li;
    size_t b2  = (right.trans) ? right.li : right.lj;

    if(a1 != a2 || b1 != b2) {
        return false;
    }
    else {

        
        for(size_t si = left.bi, soi = right.bi; si <= left.ei; ++si, ++soi) {
            for(size_t sj = left.bj, soj = right.bj; sj <= left.ej; ++sj, ++soj) {
                if((left.trans ? left.m[sj][si] : left.m[si][sj]) != (right.trans ? right.m[sj][si] : right.m[si][sj])) {
                    return false;
                }
            }
        }

        return true;
    }
};

bool operator != (const TMatrix &left, const TMatrix &right) {

    size_t a1  = (left.trans) ? left.lj : left.li;
    size_t b1 = (left.trans) ? left.li : left.lj;
    size_t a2 = (right.trans) ? right.lj : right.li;
    size_t b2  = (right.trans) ? right.li : right.lj;

    if(a1 != a2 || b1 != b2) {
        return true;
    }
    else {

        for(size_t si = left.bi, soi = right.bi; si <= left.ei; ++si, ++soi) {
            for(size_t sj = left.bj, soj = right.bj; sj <= left.ej; ++sj, ++soj) {
                if((left.trans ? left.m[sj][si] : left.m[si][sj]) != (right.trans ? right.m[sj][si] : right.m[si][sj])) {
                    return true;
                }
            }
        }

        return false;
    }
};

const TMatrix operator + (const TMatrix &left, const TMatrix &right) {
    if(left.li == right.li && left.lj == right.lj) {
        
        if(left.li != left.lj) {
            if(left.trans != right.trans) {
                std::cout << "Error: incorrect sum of matrices" << std::endl;
                std::cout << "Left matrix: " << left.li << "x" << left.lj << " | " << left.trans << std::endl;
                std::cout << "Right matrix: " << right.li << "x" << right.lj << " | " << right.trans << std::endl;
                exit(1);
            }
        }

        double** m = new double*[left.li];
        for(size_t si = 0; si < left.li; ++si) {
            m[si] = new double[left.lj];
        }
        
        for(size_t si = 0, lsi = left.bi, rsi = right.bi; si < left.li; ++si, ++lsi, ++rsi) {
            for(size_t sj = 0, lsj = left.bj, rsj = right.bj; sj < left.lj; ++sj, ++lsj, ++rsj) {
                m[si][sj] = (left.trans ? left.m[lsj][lsi] : left.m[lsi][lsj]) +
                            (right.trans ? right.m[rsj][rsi] : right.m[rsi][rsj]);
            }
        }

        return TMatrix(left.li, left.lj, m, false);
    }
    else {
        std::cout << "Error: incorrect sum of matrices" << std::endl;
        std::cout << "Left matrix: " << left.li << "x" << left.li << " | " << left.trans << std::endl;
        std::cout << "Right matrix: " << right.lj << "x" << right.lj << " | " << right.trans << std::endl;
        exit(1);
    }
}

const TMatrix operator - (const TMatrix &left, const TMatrix &right) {
    if(left.li == right.li && left.lj == right.lj) {
        
        if(left.li != left.lj) {
            if(left.trans != right.trans) {
                std::cout << "Error: incorrect matrix subtraction" << std::endl;
                std::cout << "Left matrix: " << left.li << "x" << left.li << " | " << left.trans << std::endl;
                std::cout << "Right matrix: " << right.lj << "x" << right.lj << " | " << right.trans << std::endl;
                exit(1);
            }
        }

        double** m = new double*[left.li];
        for(size_t si = 0; si < left.li; ++si) {
            m[si] = new double[left.lj];
        }
        
        for(size_t si = 0, lsi = left.bi, rsi = right.bi; si < left.li; ++si, ++lsi, ++rsi) {
            for(size_t sj = 0, lsj = left.bj, rsj = right.bj; sj < left.lj; ++sj, ++lsj, ++rsj) {
                m[si][sj] = (left.trans ? left.m[lsj][lsi] : left.m[lsi][lsj]) -
                            (right.trans ? right.m[rsj][rsi] : right.m[rsi][rsj]);;
            }
        }

        return TMatrix(left.li, left.lj, m, false);
    }
    else {
        std::cout << "Error: incorrect matrix subtraction" << std::endl;
        std::cout << "Left matrix: " << left.li << "x" << left.li << " | " << left.trans << std::endl;
        std::cout << "Right matrix: " << right.lj << "x" << right.lj << " | " << right.trans << std::endl;
        exit(1);
    }
}

const TMatrix operator * (const TMatrix &left, const TMatrix &right) {

    size_t a  = (left.trans) ? left.lj : left.li;
    size_t b1 = (left.trans) ? left.li : left.lj;
    size_t b2 = (right.trans) ? right.lj : right.li;
    size_t c  = (right.trans) ? right.li : right.lj;
    
    if(b1 == b2) {

        double** m = new double*[a];
        for(size_t si = 0; si < a; ++si) {
            m[si] = new double[c];
        }
        
        for(size_t si = 0; si < a; ++si) {
            for(size_t sj = 0; sj < c; ++sj) {
                m[si][sj] = 0;
            }
        }

        size_t nbi = 0;
        size_t nbj = 0;
        size_t nei = a - 1;
        size_t nej = c - 1;
        
        for(size_t si = nbi, lsi = left.bi; si <= nei; ++si, ++lsi) {
            for(size_t sj = nbj, rsj = right.bj; sj <= nej; ++sj, ++rsj) {
                for(size_t summ = 0, lsj = left.bj, rsi = right.bi; summ < b1; ++summ, ++lsj, ++rsi) {
                    m[si][sj] += ((left.trans)  ? left.m[lsj][lsi]  :  left.m[lsi][lsj]) *
                                 ((right.trans) ? right.m[rsj][rsi] : right.m[rsi][rsj]);
                }
            }
        }
        
        return TMatrix(a, c, m, false);
    }
    else {
        std::cout << "Error: incorrect matrix subtraction" << std::endl;
        std::cout << "Left matrix: " << a << "x" << b1 << " | " << left.trans << std::endl;
        std::cout << "Right matrix: " << b2 << "x" << c << " | " << right.trans << std::endl;
        exit(1);
    }
}

const TMatrix operator * (const double &left, const TMatrix &right) {

    size_t li = right.li;
    size_t lj = right.lj;
    
    double** m = new double*[li];
    for(size_t si = 0; si < li; ++si) {
        m[si] = new double[lj];
    }
    
    for(size_t si = 0, soi = right.bi; si < li; ++si, ++soi) {
        for(size_t sj = 0, soj = right.bj; sj < lj; ++sj, ++soj) {
            m[si][sj] = right.m[soi][soj] * left;
        }
    }
    
    return TMatrix(li, lj, m, false);
}

const TMatrix operator * (const TMatrix &left, const double &right) {

    size_t li = left.li;
    size_t lj = left.lj;
    
    double** m = new double*[li];
    for(size_t si = 0; si < li; ++si) {
        m[si] = new double[lj];
    }
    
    for(size_t si = 0, soi = left.bi; si < li; ++si, ++soi) {
        for(size_t sj = 0, soj = left.bj; sj < lj; ++sj, ++soj) {
            m[si][sj] = left.m[soi][soj] * right;
        }
    }
    
    return TMatrix(li, lj, m, false);
}

std::pair<TMatrix, TMatrix> LUDecomposition(TMatrix A, TMatrix B = TMatrix(), unsigned char type = NORMAL) {

    size_t sizeM = A.GetSA();
    double coef;
    std::vector<std::pair<size_t, size_t>> perm;

    TMatrix X(sizeM, sizeM, ZERO_MATRIX);
    TMatrix L(sizeM, sizeM, UNIT_MATRIX);
    TMatrix U(A);
    
    for(size_t k = 0; k < sizeM - 1; ++k) {
        for(size_t i = k + 1; i < sizeM; ++i) {
            
            if(U[k][k] == 0) {
                for(size_t s = k + 1; s < sizeM; ++s) {
                    if(U[s][k] != 0) {
                        U.SwapInMatrix(s, k); //После него проверка будет выдавать отрицательный результат
                        if(B.IsExist()) {
                            B.SwapInMatrix(s, k);
                        }
                        perm.push_back(std::pair<size_t, size_t>(s, k));
                        break;
                    }
                }
            }

            coef = U[i][k] / U[k][k];
            
            for(size_t j = k; j < sizeM; ++j) {
                U[i][j] = U[i][j] - coef * U[k][j];
            }

            if(B.IsExist()) {
                size_t sizeB = B.GetSB();
                for(size_t bj = 0; bj < sizeB; ++bj) {
                    B[i][bj] = B[i][bj] - coef * B[k][bj];
                }
            }
            
            L[i][k] = coef;
        }
    }
    
    if(type == NORMAL) {
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
            for(size_t pos = perm.size() - 1; pos >= 0; --pos) {
                std::cout << "Swap " << perm[pos].first << " and " << perm[pos].second << ";" << std::endl;
                T.SwapInMatrix(perm[pos].first, perm[pos].second);
                if(pos == 0) {
                    break;
                }
            }
            std::cout << "L*U = A? - ";
            if(A == (L*T)) {
                std::cout << "true" << std::endl;
                for(size_t k = 0; k < sizeM - 1; ++k) {
                    for(size_t i = k + 1; i < sizeM; ++i) {
                        U[i][k] = L[i][k];
                    }
                }
                std::cout << "LU: " << std::endl;
                std::cout << U;
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
            for(size_t k = 0; k < sizeM - 1; ++k) {
                for(size_t i = k + 1; i < sizeM; ++i) {
                    U[i][k] = L[i][k];
                }
            }
        }
    }
    else {
        
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
    }
    
    return std::pair<TMatrix, TMatrix>(((type == NORMAL) ? U : X), B);
}

TMatrix SolveSystemViaLU(TMatrix LU, TMatrix B) {

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

double GetDeterminant(TMatrix* LU) {
    size_t sizeM = LU->GetSA();
    double answer = pow(-1, LU->GetRp());
    for(size_t k = 0; k < sizeM; ++k) {
        answer *= (*LU)[k][k];
    }
    return answer;
}

TMatrix SweepMethod(TMatrix A, TMatrix B = TMatrix()) {
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

TMatrix SimpleIterations(TMatrix A, TMatrix B = TMatrix()) {

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

TMatrix SeidelMethod(TMatrix A, TMatrix B = TMatrix()) {

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

std::pair<TMatrix, TMatrix> RotationMethod(TMatrix A, double eps = 0.001) {

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
        
        A = nextU.T() * A * nextU;

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

std::pair<TMatrix, TMatrix> QRAlgorithm(TMatrix A, double eps = 0.001) {

    size_t sizeM = A.GetSA();
    TMatrix V, VT, L, H;
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
            VT = V;
            VT.Transpose();
            L = VT*V;
            H = E - (2/L[0][0])*(V*VT);
            A = H*A;
            Q *= H;
        }
    }
    return std::pair<TMatrix, TMatrix>(Q,A);
}

int main() {

    unsigned char   input;
    bool            isRun = true;

    while(isRun) {
        std::cout <<"====================" << std::endl;
        std::cout << "Task:" << std::endl;
        std::cout <<"====================" << std::endl;
        std::cout << "1. LU" << std::endl;
        std::cout << "2. Sweep Method" << std::endl;
        std::cout << "3. Iterations" << std::endl;
        std::cout << "4. Eigenvector and Eigenvalue" << std::endl;
        std::cout << "5. QR" << std::endl;
        std::cout << "6. Exit" << std::endl;
        std::cout <<"====================" << std::endl;

        std::cin >> input;
        switch(input) {
            case '1':
            {
                std::cout << "Input matrix A:" << std::endl;
                TMatrix A(std::cin);
                std::cout << "Input matrix B:" << std::endl;
                TMatrix B(std::cin);
                
                TMatrix X;

                std::pair<TMatrix, TMatrix> LUB, XB;
                LUB = LUDecomposition(A, B);
                std::cout << "Vector LU:" << std::endl;
                std::cout << LUB.first;
                std::cout << "Vector B:" << std::endl;
                std::cout << LUB.second;
                X = SolveSystemViaLU(LUB.first, LUB.second);
                std::cout << "Vector X:" << std::endl;
                std::cout << X;

                unsigned char cont;
                std::cout << "Check? - ";
                std::cin >> cont;
                if(cont == 'y') {
                    std::cout << "Check:" << std::endl;
                    std::cout << "Vector B: " << std::endl;
                    std::cout << B;
                    std::cout << "Vector A*X: " << std::endl;
                    std::cout << A*X;
                }
                else {
                    std::cout << "End!" << std::endl;
                }
                std::cout << "Det A = " << GetDeterminant(&(LUB.first)) << std::endl;
                std::cout << "Det (check) A = " << A.GetDeterminant() << std::endl;

                TMatrix E(A.GetSA(), A.GetSB(), UNIT_MATRIX);
                XB = LUDecomposition(A, E, REVERSE);

                std::cout << "A^(-1):" << std::endl;
                std::cout << XB.first;

                std::cout << "Check? - ";
                std::cin >> cont;
                if(cont == 'y') {
                    std::cout << "Check:" << std::endl;
                    std::cout << "Matrix A*A^(-1): " << std::endl;
                    std::cout << A*XB.first;
                }
                else {
                    std::cout << "End!" << std::endl;
                }
                break;
            }
            case '2':
            {
                std::cout << "Input matrix A:" << std::endl;
                TMatrix A(std::cin);
                std::cout << "Input matrix B:" << std::endl;
                TMatrix B(std::cin);
                
                TMatrix X;

                X = SweepMethod(A, B);
                std::cout << "Vector X:" << std::endl;
                std::cout << X;

                unsigned char cont;
                std::cout << "Check? - ";
                std::cin >> cont;
                if(cont == 'y') {
                    std::cout << "Check:" << std::endl;
                    std::cout << "Vector B: " << std::endl;
                    std::cout << B;
                    std::cout << "Vector A*X: " << std::endl;
                    std::cout << A*X;
                }
                else {
                    std::cout << "End!" << std::endl;
                }
                break;
            }
            case '3':
            {
                std::cout << "Input matrix A:" << std::endl;
                TMatrix A(std::cin);
                std::cout << "Input matrix B:" << std::endl;
                TMatrix B(std::cin);
                
                TMatrix X;

                X = SimpleIterations(A, B);
                std::cout << "Vector X:" << std::endl;
                std::cout << X;

                unsigned char cont;
                std::cout << "Check? - ";
                std::cin >> cont;
                if(cont == 'y') {
                    std::cout << "Check:" << std::endl;
                    std::cout << "Vector B: " << std::endl;
                    std::cout << B;
                    std::cout << "Vector A*X: " << std::endl;
                    std::cout << A*X;
                }
                else {
                    std::cout << "End!" << std::endl;
                }

                X = SeidelMethod(A, B);
                std::cout << "Vector X:" << std::endl;
                std::cout << X;

                std::cout << "Check? - ";
                std::cin >> cont;
                if(cont == 'y') {
                    std::cout << "Check:" << std::endl;
                    std::cout << "Vector B: " << std::endl;
                    std::cout << B;
                    std::cout << "Vector A*X: " << std::endl;
                    std::cout << A*X;
                }
                else {
                    std::cout << "End!" << std::endl;
                }
                break;
            }
            case '4':
            {
                std::cout << "Input matrix A:" << std::endl;
                TMatrix A(std::cin);
                double eps;
                size_t sizeM;
                std::cout << "Input epsilon: " << std::endl;
                std::cin >> eps;
                
                std::pair<TMatrix, TMatrix> answer;

                answer = RotationMethod(A, eps);
                std::cout << "Eigenvalues:" << std::endl;

                sizeM = answer.first.GetSA();
                
                for(size_t k = 0; k < sizeM; ++k){
                    std::cout << '[' << answer.first[k][k] << ']';
                    if(k + 1 != sizeM) {
                        std::cout << ", ";
                    }
                }
                std::cout << std::endl;
                
                std::cout << "Eigenvectors matrix:" << std::endl;
                std::cout << answer.second;

                unsigned char cont;
                std::cout << "Check? - ";
                std::cin >> cont;
                TMatrix V1, V2;
                if(cont == 'y') {
                    std::cout << "Check:" << std::endl;
                    for(size_t j1 = 0; j1 < sizeM; ++j1) {
                        for(size_t j2 = j1 + 1; j2 < sizeM; ++j2) {
                            V1 = TMatrix(answer.second, 0, j1, sizeM - 1, j1);
                            V2 = TMatrix(answer.second, 0, j2, sizeM - 1, j2);
                            std::cout << "(V" << j1 << ", V" << j2 << ") = " << V1.T()*V2;
                        }
                    }
                }
                else {
                    std::cout << "End!" << std::endl;
                }
                break;
            }
            case '5':
            {
                std::cout << "Input matrix A:" << std::endl;
                TMatrix A(std::cin);
                double eps;
                size_t sizeM;
                std::cout << "Input epsilon: " << std::endl;
                std::cin >> eps;
                std::pair<TMatrix, TMatrix> answer;

                answer = QRAlgorithm(A, eps);
                std::cout << "Eigenvalues:" << std::endl;
                sizeM = answer.second.GetSA();
                
                for(size_t k = 0; k < sizeM; ++k){
                    std::cout << '[' << answer.second[k][k] << ']';
                    if(k + 1 != sizeM) {
                        std::cout << ", ";
                    }
                }
                std::cout << std::endl;

                unsigned char cont;
                std::cout << "Check? - ";
                std::cin >> cont;
                if(cont == 'y') {
                    std::cout << "Check:" << std::endl;
                    std::cout << "A:" << std::endl;
                    std::cout << A;
                    std::cout << "Q:" << std::endl;
                    std::cout << answer.first;
                    std::cout << "R:" << std::endl;
                    std::cout << answer.second;
                    std::cout << "Q*R:" << std::endl;
                    std::cout << answer.first * answer.second;
                    
                }
                else {
                    std::cout << "End!" << std::endl;
                }
                break;
            }
            case '6':
                isRun = false;
                break;
            default:
                std::cout << "Please try again!" << std::endl;
        }
    }
    
    return 0;
}
