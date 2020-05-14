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

        TMatrix(): trans(false), a(0), b(0), rp(0), m(nullptr) {}

        TMatrix(const TMatrix &orig) {
            a = orig.a;
            b = orig.b;
            rp = orig.rp;
            trans = orig.trans;

            m = new double*[a];
            for(size_t step = 0; step < a; ++step) {
                m[step] = new double[b];
            }

            for(size_t stepX = 0; stepX < a; ++stepX) {
                for(size_t stepY = 0; stepY < b; ++stepY) {
                    m[stepX][stepY] = orig.m[stepX][stepY];
                }
            }
        }

        TMatrix(const TMatrix &orig, size_t i1, size_t j1, size_t i2, size_t j2, bool copy = true) {
            a = i2 - i1 + 1;
            b = j2 - j1 + 1;
            rp = 0;
            trans = 0;
            if(copy) {
                m = new double*[a];
                for(size_t step = 0; step < a; ++step) {
                    m[step] = new double[b];
                }
    
                for(size_t stepX = i1; stepX <= i2; ++stepX) {
                    for(size_t stepY = j1; stepY <= j2; ++stepY) {
                        m[stepX][stepY] = orig.m[stepX][stepY];
                    }
                }
            }
            else {
                m = orig.m;
            }
        }
        
        TMatrix(std::istream &is) {

            std::cout << "Enter the number of lines: ";
            is >> a;
            std::cout << "Enter the number of columns: ";
            is >> b;
            
            rp = 0;
            
            m = new double*[a];
            for(size_t step = 0; step < a; ++step) {
                m[step] = new double[b];
            }
            std::cout << "Enter items of matrix:" << std::endl;
            for(size_t stepX = 0; stepX < a; ++stepX) {
                for(size_t stepY = 0; stepY < b; ++stepY) {
                    is >> m[stepX][stepY];
                }
            }

            trans = false;
        }
        
        TMatrix(size_t i, size_t j, double **matrix = nullptr, bool copy = true) {

            a = i;
            b = j;

            rp = 0;
            trans = false;
            
            if(copy) {
                m = new double*[a];
                for(size_t step = 0; step < a; ++step) {
                    m[step] = new double[b];
                }
    
                if(matrix == nullptr) {
                    for(size_t stepX = 0; stepX < a; ++stepX) {
                        for(size_t stepY = 0; stepY < b; ++stepY) {
                            m[stepX][stepY] = 0;
                        }
                    }
                }
                else {
                    for(size_t stepX = 0; stepX < a; ++stepX) {
                        for(size_t stepY = 0; stepY < b; ++stepY) {
                            m[stepX][stepY] = matrix[stepX][stepY];
                        }
                    }
                }
            }
            else {
                m = matrix;
            }
        };

        TMatrix(size_t i, size_t j, size_t type) {
            a = i;
            b = j;

            rp = 0;
            
            m = new double*[a];
            for(size_t step = 0; step < a; ++step) {
                m[step] = new double[b];
            }

            switch(type) {
                case ZERO_MATRIX:
                    for(size_t stepX = 0; stepX < a; ++stepX) {
                        for(size_t stepY = 0; stepY < b; ++stepY) {
                            m[stepX][stepY] = 0;
                        }
                    }
                    break;
                case UNIT_MATRIX:
                    for(size_t stepX = 0; stepX < a; ++stepX) {
                        for(size_t stepY = 0; stepY < b; ++stepY) {
                            m[stepX][stepY] = (stepX == stepY ? 1 : 0);
                        }
                    }
                    break;
                default:
                    for(size_t stepX = 0; stepX < a; ++stepX) {
                        for(size_t stepY = 0; stepY < b; ++stepY) {
                            m[stepX][stepY] = 0;
                        }
                    }
                    break;
            }
            trans = false;
        };
        
        ~TMatrix() {
            
            for(size_t stepX = 0; stepX < a; ++stepX) {
                delete m[stepX];
            }
            delete m;
        }

        double CalcNorm(unsigned char type = TYPE_2) {
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
        
        void SwapInMatrix(size_t i1, size_t i2, size_t type = STRING) {
            double tmp;
            if(type == STRING) {
                for(size_t step = 0; step < b; ++step) {
                    tmp = m[i1][step];
                    m[i1][step] = m[i2][step];
                    m[i2][step] = tmp;
                }   
            }
            else {
                for(size_t step = 0; step < a; ++step) {
                    tmp = m[step][i1];
                    m[step][i1] = m[step][i2];
                    m[step][i2] = tmp;
                }
            }
            ++rp;
            return;
        }
        
        void Print() {
            if(!trans) {
                if(m != nullptr) {
                    for(size_t stepX = 0; stepX < a; ++stepX) {
                        for(size_t stepY = 0; stepY < b; ++stepY) {
                            std::cout << m[stepX][stepY];
                            if(stepY != b - 1) {
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
            else {
                if(m != nullptr) {
                    for(size_t stepX = 0; stepX < b; ++stepX) {
                        for(size_t stepY = 0; stepY < a; ++stepY) {
                            std::cout << m[stepY][stepX];
                            if(stepY != a - 1) {
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
        };

        double GetDeterminant() {

            double answer = 0;
            
            if(a != b) {
                std::cout << "Error: the determinant of a non-square matrix cannot be calculated!" << std::endl;
                std::cout << a << 'x' << b << ':' << std::endl;
                std::cout << *this;
                exit(1);
            }
            else {
                if(a == 1) {
                    return m[0][0];
                }
                else if(a == 2) {
                    answer = m[0][0]*m[1][1] - m[0][1]*m[1][0];
                }
                else if (a == 3) {
                    answer = m[0][0]*m[1][1]*m[2][2] - m[0][0]*m[1][2]*m[2][1] - m[0][1]*m[1][0]*m[2][2]
                           + m[0][1]*m[1][2]*m[2][0] + m[0][2]*m[1][0]*m[2][1] - m[0][2]*m[1][1]*m[2][0];
                }
                else {

                    size_t* leadElem = new size_t[b];
                    
                    for(size_t j = 0; j < b; ++j) {
                        leadElem[0] = j;
                        answer += pow(-1, j) * m[0][j] * this->recDeterminant(leadElem, 1);
                    }

                    delete leadElem;
                }
            }

            return answer;
        }

        size_t GetSizeA() {
            return a;
        }

        size_t GetSizeB() {
            return b;
        }

        size_t GetRp() {
            return rp;
        }

        bool IsExist() {
            return (m != nullptr);
        };

        void Transpose() {
            trans = (trans) ? false : true;
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
            if(this->a == right.a && this->b == right.b && this->trans == right.trans) {

                for(size_t stepX = 0; stepX < this->a; ++stepX) {
                    for(size_t stepY = 0; stepY < this->b; ++stepY) {
                        this->m[stepX][stepY] += right.m[stepX][stepY];
                    }
                }
        
                return *this;
            }
            else {
                std::cout << "Error: incorrect sum of matrices" << std::endl;
                std::cout << "Left matrix: " << this->a << "x" << this->b << " | " << this->trans << std::endl;
                std::cout << "Right matrix: " << right.a << "x" << right.b << " | " << right.trans << std::endl;
                exit(1);
            }
        };
        
        TMatrix& operator -= (const TMatrix &right) {
            if(this->a == right.a && this->b == right.b && this->trans == right.trans) {

                for(size_t stepX = 0; stepX < this->a; ++stepX) {
                    for(size_t stepY = 0; stepY < this->b; ++stepY) {
                        this->m[stepX][stepY] -= right.m[stepX][stepY];
                    }
                }
        
                return *this;
            }
            else {
                std::cout << "Error: incorrect sum of matrices" << std::endl;
                std::cout << "Left matrix: " << this->a << "x" << this->b << " | " << this->trans << std::endl;
                std::cout << "Right matrix: " << right.a << "x" << right.b << " | " << right.trans << std::endl;
                exit(1);
            }
        };
        
        TMatrix& operator *= (const TMatrix &right) {
            
            size_t a  = (this->trans) ? this->b : this->a;
            size_t b1 = (this->trans) ? this->a : this->b;
            size_t b2 = (right.trans) ? right.b : right.a;
            size_t c  = (right.trans) ? right.a : right.b;
            
            if(b1 == b2) {
        
                double** tmp = new double*[a];
                for(size_t stepX = 0; stepX < a; ++stepX) {
                    tmp[stepX] = new double[c];
                }
                
                for(size_t stepX = 0; stepX < a; ++stepX) {
                    for(size_t stepY = 0; stepY < c; ++stepY) {
                        tmp[stepX][stepY] = 0;
                    }
                }
        
                for(size_t stepX = 0; stepX < a; ++stepX) {
                    for(size_t stepY = 0; stepY < c; ++stepY) {
                        for(size_t summ = 0; summ < b1; ++summ) {
                            tmp[stepX][stepY] += ((this->trans) ? this->m[summ][stepX] : this->m[stepX][summ]) *
                                               ((this->trans) ? right.m[stepY][summ] : right.m[summ][stepY]);
                        }
                    }
                }

                for(size_t stepX = 0; stepX < this->a; ++stepX) {
                    delete m[stepX];
                }
                delete m;

                m = tmp;
                
                return *this;
            }
            else {
                std::cout << "Error: incorrect matrix subtraction" << std::endl;
                std::cout << "Left matrix: " << a << "x" << b1 << " | " << this->trans << std::endl;
                std::cout << "Right matrix: " << b2 << "x" << c << " | " << right.trans << std::endl;
                exit(1);
            }
        };

        TMatrix& operator = (const TMatrix& right) {

            if(this == &right) {
                return *this;
            }

            if(m != nullptr) {
                for(size_t stepX = 0; stepX < a; ++stepX) {
                    delete m[stepX];
                }
                delete m;
            }
            
            a = right.a;
            b = right.b;
            rp = right.rp;
            trans = right.trans;

            m = new double*[a];
            for(size_t step = 0; step < a; ++step) {
                m[step] = new double[b];
            }
            
            for(size_t stepX = 0; stepX < a; ++stepX) {
                for(size_t stepY = 0; stepY < b; ++stepY) {
                    m[stepX][stepY] = right.m[stepX][stepY];
                }
            }

            return *this;
        }

        double* operator [] (const size_t index) {
            return m[index];
        }
        
    private:

        bool            trans;
        
        size_t          a;
        size_t          b;

        size_t          rp;

        double**        m;

        long long recDeterminant(size_t* leadElem, size_t nstr) {
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
                size_t      nItem = 1;
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
};

std::ostream& operator << (std::ostream &out, const TMatrix &matrix) {
    if(!matrix.trans) {
        if(matrix.m != nullptr) {
            for(size_t stepX = 0; stepX < matrix.a; ++stepX) {
                for(size_t stepY = 0; stepY < matrix.b; ++stepY) {
                    out << matrix.m[stepX][stepY];
                    if(stepY != matrix.b - 1) {
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
            for(size_t stepX = 0; stepX < matrix.b; ++stepX) {
                for(size_t stepY = 0; stepY < matrix.a; ++stepY) {
                    out << matrix.m[stepY][stepX];
                    if(stepY != matrix.a - 1) {
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

    size_t a1  = (left.trans) ? left.b : left.a;
    size_t b1 = (left.trans) ? left.a : left.b;
    size_t a2 = (right.trans) ? right.b : right.a;
    size_t b2  = (right.trans) ? right.a : right.b;

    if(a1 != a2 || b1 != b2) {
        return false;
    }
    else {
        for(size_t i = 0; i < a1; ++i) {
            for(size_t j = 0; j < b1; ++j) {
                if(left.m[i][j] != right.m[i][j]) {
                    return false;
                }
            }
        }

        return true;
    }
};

bool operator != (const TMatrix &left, const TMatrix &right) {

    size_t a1  = (left.trans) ? left.b : left.a;
    size_t b1 = (left.trans) ? left.a : left.b;
    size_t a2 = (right.trans) ? right.b : right.a;
    size_t b2  = (right.trans) ? right.a : right.b;

    if(a1 != a2 || b1 != b2) {
        return true;
    }
    else {
        for(size_t i = 0; i < a1; ++i) {
            for(size_t j = 0; j < b1; ++j) {
                if(left.m[i][j] != right.m[i][j]) {
                    return true;
                }
            }
        }

        return false;
    }
};

const TMatrix operator + (const TMatrix &left, const TMatrix &right) {
    if(left.a == right.a && left.b == right.b && left.trans == right.trans) {

        double** m = new double*[left.a];
        for(size_t stepX = 0; stepX < left.a; ++stepX) {
            m[stepX] = new double[left.b];
        }
        
        for(size_t stepX = 0; stepX < left.a; ++stepX) {
            for(size_t stepY = 0; stepY < left.b; ++stepY) {
                m[stepX][stepY] = left.m[stepX][stepY] + right.m[stepX][stepY];
            }
        }

        return TMatrix(left.a, left.b, m, true);
    }
    else {
        std::cout << "Error: incorrect sum of matrices" << std::endl;
        std::cout << "Left matrix: " << left.a << "x" << left.b << " | " << left.trans << std::endl;
        std::cout << "Right matrix: " << right.a << "x" << right.b << " | " << right.trans << std::endl;
        exit(1);
    }
}

const TMatrix operator - (const TMatrix &left, const TMatrix &right) {
    if(left.a == right.a && left.b == right.b && left.trans == right.trans) {

        double** m = new double*[left.a];
        for(size_t stepX = 0; stepX < left.a; ++stepX) {
            m[stepX] = new double[left.b];
        }
        
        for(size_t stepX = 0; stepX < left.a; ++stepX) {
            for(size_t stepY = 0; stepY < left.b; ++stepY) {
                m[stepX][stepY] = left.m[stepX][stepY] - right.m[stepX][stepY];
            }
        }

        return TMatrix(left.a, left.b, m, true);
    }
    else {
        std::cout << "Error: incorrect matrix subtraction" << std::endl;
        std::cout << "Left matrix: " << left.a << "x" << left.b << " | " << left.trans << std::endl;
        std::cout << "Right matrix: " << right.a << "x" << right.b << " | " << right.trans << std::endl;
        exit(1);
    }
}

const TMatrix operator * (const TMatrix &left, const TMatrix &right) {

    size_t a  = (left.trans) ? left.b : left.a;
    size_t b1 = (left.trans) ? left.a : left.b;
    size_t b2 = (right.trans) ? right.b : right.a;
    size_t c  = (right.trans) ? right.a : right.b;
    
    if(b1 == b2) {

        double** m = new double*[a];
        for(size_t stepX = 0; stepX < a; ++stepX) {
            m[stepX] = new double[c];
        }
        
        for(size_t stepX = 0; stepX < a; ++stepX) {
            for(size_t stepY = 0; stepY < c; ++stepY) {
                m[stepX][stepY] = 0;
            }
        }

        for(size_t stepX = 0; stepX < a; ++stepX) {
            for(size_t stepY = 0; stepY < c; ++stepY) {
                for(size_t summ = 0; summ < b1; ++summ) {
                    m[stepX][stepY] += ((left.trans) ? left.m[summ][stepX] : left.m[stepX][summ]) *
                                       ((right.trans) ? right.m[stepY][summ] : right.m[summ][stepY]);
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

    size_t a = right.a;
    size_t b = right.b;
    
    double** m = new double*[a];
    for(size_t stepX = 0; stepX < a; ++stepX) {
        m[stepX] = new double[b];
    }
    
    for(size_t stepX = 0; stepX < a; ++stepX) {
        for(size_t stepY = 0; stepY < b; ++stepY) {
            m[stepX][stepY] = right.m[stepX][stepY] * left;
        }
    }
    
    return TMatrix(a, b, m, false);
}

const TMatrix operator * (const TMatrix &left, const double &right) {

    size_t a = left.a;
    size_t b = left.b;
    
    double** m = new double*[a];
    for(size_t stepX = 0; stepX < a; ++stepX) {
        m[stepX] = new double[b];
    }
    
    for(size_t stepX = 0; stepX < a; ++stepX) {
        for(size_t stepY = 0; stepY < b; ++stepY) {
            m[stepX][stepY] = left.m[stepX][stepY] * right;
        }
    }
    
    return TMatrix(a, b, m, false);
}

std::pair<TMatrix, TMatrix> LUDecomposition(TMatrix A, TMatrix B = TMatrix(), unsigned char type = NORMAL) {

    size_t sizeM = A.GetSizeA();
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
                size_t sizeB = B.GetSizeB();
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

    size_t sizeM = LU.GetSizeA();
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
    size_t sizeM = LU->GetSizeA();
    double answer = pow(-1, LU->GetRp());
    for(size_t k = 0; k < sizeM; ++k) {
        answer *= (*LU)[k][k];
    }
    return answer;
}

TMatrix SweepMethod(TMatrix A, TMatrix B = TMatrix()) {
    size_t sizeM = A.GetSizeA();
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
    size_t sizeM = A.GetSizeA();
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
    size_t sizeM = A.GetSizeA();
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

    size_t sizeM = A.GetSizeA();
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
        
        nextU.Transpose();
        
        A = nextU * A;

        nextU.Transpose();

        A = A * nextU;

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

    size_t sizeM = A.GetSizeA();
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

                TMatrix E(A.GetSizeA(), A.GetSizeB(), UNIT_MATRIX);
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

                sizeM = answer.first.GetSizeA();
                
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
                            V1.Transpose();
                            V2 = TMatrix(answer.second, 0, j2, sizeM - 1, j2);
                            std::cout << "(V" << j1 << ", V" << j2 << ") = " << V1*V2;
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
                sizeM = answer.second.GetSizeA();
                
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
