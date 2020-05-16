#include "aproxcalc.hpp"

TMatrix NewtonMethod(TMatrix& X0, double eps) {

    TMatrix  A(2, 2);
    TMatrix  F(2, 1);
    TMatrix DX(2, 1);
    TMatrix  X(X0);

    F[0][0] = -SF1(X0[0][0], X0[1][0]);
    F[1][0] = -SF2(X0[0][0], X0[1][0]);
    A[0][0] = dSF11(X0[0][0], X0[1][0]);
    A[0][1] = dSF12(X0[0][0], X0[1][0]);
    A[1][0] = dSF21(X0[0][0], X0[1][0]);
    A[1][1] = dSF22(X0[0][0], X0[1][0]);

    std::pair<TMatrix, TMatrix> ans = LUDecomposition(A, F, false);
    DX = SolveSystemViaLU(ans.first, ans.second);
    X += DX;
    if(DX.CalcNorm() < eps) {
        return X;
    }
    else {
        for(size_t iter = 0; DX.CalcNorm() > eps; ++iter) {

            F[0][0] = -SF1(X[0][0], X[1][0]);
            F[1][0] = -SF2(X[0][0], X[1][0]);
            A[0][0] = dSF11(X[0][0], X[1][0]);
            A[0][1] = dSF12(X[0][0], X[1][0]);
            A[1][0] = dSF21(X[0][0], X[1][0]);
            A[1][1] = dSF22(X[0][0], X[1][0]);

            ans = LUDecomposition(A, F, false);
            DX = SolveSystemViaLU(ans.first, ans.second);
            X += DX;
        }
    }
    return X;
}

TMatrix SimpleIterations(TMatrix& X0, TMatrix A, TMatrix B, double eps) {

    TMatrix Delta(B);
    TMatrix DFi(2,2);
    Delta -= A;
    Delta /= (POINTS - 1);
    TMatrix X(A);
    double norm = 0, mnorm = 0;
    for(size_t pi = 0; pi < (POINTS - 1); ++pi) {
        for(size_t pj = 0; pj < (POINTS - 1); ++pj) {
            DFi[0][0] = dSF11(X[0][0], X[1][0]);
            DFi[0][1] = dSF12(X[0][0], X[1][0]);
            DFi[1][0] = dSF21(X[0][0], X[1][0]);
            DFi[1][1] = dSF22(X[0][0], X[1][0]);
            norm = DFi.CalcNorm();
            mnorm = (norm > mnorm ? norm : mnorm);
            X[1][0] += Delta[1][0];
        }
        X[0][0] += Delta[0][0];
    }

    if(mnorm > 0) {
        std::cout << "Failed" << std::endl;
        //return TMatrix(2, 1);
    }
    
    TMatrix NX(2, 1);
    TMatrix DX(2, 1);
    X = X0;

    NX[0][0] = SFi1(X[0][0], X[1][0]);
    NX[1][0] = SFi2(X[0][0], X[1][0]);
    DX = NX - X;
    if(DX.CalcNorm() < eps) {
        return X;
    }
    else {
        for(size_t iter = 0; DX.CalcNorm() > eps; ++iter) {
            X = NX;
            NX[0][0] = SFi1(X[0][0], X[1][0]);
            NX[1][0] = SFi2(X[0][0], X[1][0]);
            DX = NX - X;
        }
    }
    return X;
}

double NewtonMethod(double a, double b, double begin, double eps) {

    if(Func(a)*Func(b) > 0) {
        std::cout << "Error: F(a)*F(b) > 0" << std::endl;
        return 0;
    }
    
    if(begin < a || begin > b) {
        std::cout << "Error: x0 are out of borders!" << std::endl;
        std::cout << "Take x0 = " << a << std::endl;
        begin = a;
        if(dFunc(begin)*ddFunc(begin) <= 0) {
            std::cout << "Error: dF(a)*d(dF(a)) <= 0!" << std::endl;
            std::cout << "Take x0 = " << b << std::endl;
            begin = b;
            if(dFunc(begin)*ddFunc(begin) <= 0) {
                std::cout << "Error: dF(x0)*d(dF(x0)) <= 0" << std::endl;
                return 0;
            }
        }
    }
    else if(dFunc(begin)*ddFunc(begin) <= 0) {
        std::cout << "Error: dF(x0)*d(dF(x0)) <= 0" << std::endl;
        return 0;
    }

    double x = begin;
    double nx = x - Func(x) / dFunc(x);
    double delta = nx - x;
    
    delta = (delta >= 0) ? delta : -delta;

    for(size_t iter = 0; delta > eps; ++iter) {

        x = nx;
        nx = x - Func(x) / dFunc(x);

        delta = nx - x;
        delta = (delta >= 0) ? delta : -delta;
    }

    return nx;
}

double SimpleIterations(double a, double b, double begin, double eps) {

    if(begin < a || begin > b) {
        std::cout << "Error: x0 are out of borders!" << std::endl;
        std::cout << "Take x0 = " << a << std::endl;
        begin = a;
    }

    double (*Fi[])(double) = {&Fi1, &Fi2};
    double (*dFi[])(double) = {&dFi1, &dFi2};
    double x = a;
    double dx = (b - a) / (POINTS - 1);    
    double nx;
    double delta;
    double point[4];
    double dMax[2]  = {0, 0};
    bool   check[2] = {true, true};
    
    for(size_t p = 1; p < POINTS; ++p) {

        point[0] = Fi[0](x);
        point[1] = Fi[1](x);
        point[2] = dFi[0](x);
        point[3] = dFi[1](x);
        point[2] = (point[2] >= 0) ? point[2] : -point[2];
        point[3] = (point[3] >= 0) ? point[3] : -point[3];
        dMax[0] = (dMax[0] < point[2]) ? point[2] : dMax[0];
        dMax[1] = (dMax[1] < point[3]) ? point[3] : dMax[1];
        
        if(point[0] < a || point[0] > b) {
            check[0] = false;
        }

        if(point[1] < a || point[1] > b) {
            check[1] = false;
        }

        x += dx;
    }

    if(dMax[0] > 1) {
        check[0] = false;
    }

    if(dMax[1] > 1) {
        check[1] = false;
    }

    if(check[0] || check[1]) {
        std::cout << "All conditions are completed!" << std::endl;

        x = begin;
        
        if(check[0]) {
            
            
            nx = Fi[0](x);
            delta = nx - x;
            delta = (delta >= 0) ? delta : -delta;
        
            for(size_t iter = 0; delta > eps; ++iter) {
        
                x = nx;
                nx = Fi[0](x);
        
                delta = nx - x;
                delta = (delta >= 0) ? delta : -delta;
                
            }
        }
        else {

            nx = Fi[1](x);
            delta = nx - x;
            delta = (delta >= 0) ? delta : -delta;
        
            for(size_t iter = 0; delta > eps; ++iter) {
        
                x = nx;
                nx = Fi[1](x);
        
                delta = nx - x;
                delta = (delta >= 0) ? delta : -delta;
                
            }
        }
    }
    else {
        std::cout << "Error: bad borders!" << std::endl;
    }

    return nx;
}
