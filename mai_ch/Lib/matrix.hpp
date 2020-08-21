#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "globals.hpp"

//=========================================================================================================

class TMatrix {
    public:

        TMatrix();
        TMatrix(const TMatrix &orig);
        TMatrix(const TMatrix &orig, bool t);
        TMatrix(const TMatrix &orig, size_t i1, size_t j1, size_t i2, size_t j2);
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
        void    PrintPart(size_t i1, size_t j1, size_t i2, size_t j2) const;
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

        
        TMatrix&    operator += (const TMatrix &right);
        TMatrix&    operator -= (const TMatrix &right);
        TMatrix&    operator *= (const TMatrix &right);
        TMatrix&    operator *= (const double  &right);
        TMatrix&    operator /= (const double  &right);
        TMatrix&    operator =  (const TMatrix &right);
        double*     operator [] (const size_t index) const;
        
    private:

        bool                                    trans;
        bool                                    own;
        
        size_t                                  a;
        size_t                                  b;
        
        std::vector<std::pair<size_t, size_t>>  rp;
        std::vector<unsigned char>              trp;

        double**                                m;

        long long recDeterminant(size_t* leadElem, size_t nstr);
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

//=========================================================================================================

std::pair<TMatrix, TMatrix> LUDecomposition(TMatrix& A, TMatrix B, bool checkLU = true);
TMatrix                     InverseMatrix(TMatrix& A);
TMatrix                     SolveSystemViaLU(TMatrix& LU, TMatrix B);
double                      GetDeterminant(TMatrix& LU);
TMatrix                     SweepMethod(TMatrix& A, TMatrix& B);
TMatrix                     SimpleIterations(TMatrix A, TMatrix B);
TMatrix                     SeidelMethod(TMatrix A, TMatrix B);
std::pair<TMatrix, TMatrix> RotationMethod(TMatrix A, double eps = EPSILON);
std::pair<TMatrix, TMatrix> QRAlgorithm(TMatrix A, double eps = EPSILON);

//=========================================================================================================
#endif
