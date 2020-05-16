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

enum Reverse {
    VIA_LU,
    VIA_M
};

enum NormTypes {
    TYPE_1,
    TYPE_2,
    TYPE_C
};

enum MenuTypes {
    BEGIN_MENU,
    TASK_MENU
};

enum Swap {
    BACK,
    FORWARD
};

class TMatrix {
    public:

        TMatrix();
        TMatrix(const TMatrix &orig);
        TMatrix(const TMatrix &orig, bool t);
        TMatrix(const TMatrix &orig, size_t i1, size_t j1, size_t i2, size_t j2, bool copy = true);
        TMatrix(std::istream &is);
        TMatrix(size_t i, size_t j, double **matrix, bool copy = true);
        TMatrix(size_t i, size_t j, size_t type = ZERO_MATRIX);
        
        ~TMatrix();

        double  CalcNorm(unsigned char type = TYPE_2);
        void    SwapInMatrix(size_t i1, size_t i2, unsigned char type = STRING);
        void    SwapBackward(unsigned char type);
        double  GetDeterminant();
        size_t  GetSA() const;
        size_t  GetSB() const;
        size_t  GetRp() const;
        bool    IsExist() const;
        void    Transpose();
        TMatrix T();
        
        friend const    TMatrix         operator +  (const TMatrix &left, const TMatrix &right);
        friend const    TMatrix         operator -  (const TMatrix &left, const TMatrix &right);
        friend const    TMatrix         operator *  (const TMatrix &left, const TMatrix &right);
        friend const    TMatrix         operator *  (const double &left, const TMatrix &right);
        friend const    TMatrix         operator *  (const TMatrix &left, const double &right);
        friend const    TMatrix         operator /  (const TMatrix &left, const double &right);
        friend          bool            operator == (const TMatrix &left, const TMatrix &right);
        friend          bool            operator != (const TMatrix &left, const TMatrix &right);
        friend          std::ostream&   operator << (std::ostream &out, const TMatrix &matrix);

        TMatrix operator += (const TMatrix &right) {

            size_t a1 = (trans) ?  lj : li;
            size_t b1 = (trans) ?  li : lj;
            size_t a2 = (right.trans) ? right.lj : right.li;
            size_t b2 = (right.trans) ? right.li : right.lj;
            
            if(a1 == a2 && b1 == b2) {
        
                if(!ownM) {
                    CreateOwnTable();
                }
                
                size_t rbi = right.bi;
                size_t rbj = right.bj;
                for(size_t si = 0; si < a1; ++si) {
                    for(size_t sj = 0; sj < b1; ++sj) {
                        (trans ? m[sj][si] : m[si][sj]) += (right.trans ? right.m[rbi + sj][rbj + si] : right.m[rbi + si][rbj + sj]);
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
        };
        
        TMatrix&    operator -= (const TMatrix &right);
        TMatrix&    operator *= (const TMatrix &right);
        TMatrix&    operator *= (const double &right);
        TMatrix&    operator /= (const double &right);
        TMatrix&    operator =  (const TMatrix& right);
        double*     operator [] (const size_t index) const;
        
    private:

        bool                                    trans;
        bool                                    ownM;

        size_t                                  bi;
        size_t                                  bj;
        size_t                                  ei;
        size_t                                  ej;
        size_t                                  li;
        size_t                                  lj;
        
        std::vector<std::pair<size_t, size_t>>  rp;
        std::vector<unsigned char>              trp;

        double**                                m;

        long long recDeterminant(size_t* leadElem, size_t nstr);
        void CreateOwnTable();
};

        std::ostream&   operator << (std::ostream &out, const TMatrix &matrix);
        bool            operator == (const TMatrix &left, const TMatrix &right);
        bool            operator != (const TMatrix &left, const TMatrix &right);
const   TMatrix         operator +  (const TMatrix &left, const TMatrix &right);
const   TMatrix         operator -  (const TMatrix &left, const TMatrix &right);
const   TMatrix         operator *  (const TMatrix &left, const TMatrix &right);
const   TMatrix         operator *  (const double &left, const TMatrix &right);
const   TMatrix         operator *  (const TMatrix &left, const double &right);
const   TMatrix         operator /  (const TMatrix &left, const double &right);

std::pair<TMatrix, TMatrix> LUDecomposition(TMatrix& A, TMatrix B, bool checkLU = true);
TMatrix                     InverseMatrix(TMatrix& A);
TMatrix                     SolveSystemViaLU(TMatrix& LU, TMatrix B);
double                      GetDeterminant(TMatrix& LU);
TMatrix                     SweepMethod(TMatrix& A, TMatrix& B);
TMatrix                     SimpleIterations(TMatrix A, TMatrix B);
TMatrix                     SeidelMethod(TMatrix A, TMatrix B);
std::pair<TMatrix, TMatrix> RotationMethod(TMatrix A, double eps = 0.001);
std::pair<TMatrix, TMatrix> QRAlgorithm(TMatrix A, double eps = 0.001);
