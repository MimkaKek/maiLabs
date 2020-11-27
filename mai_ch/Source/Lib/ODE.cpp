#include "ODE.hpp"
#include "functions.hpp"

TMatrix Euler(double (*F)(double, double, double),
              double (*G)(double, double, double),
              double x0, double y0, double dy0, double a, double b, double h){

    double tmp = b - a;
    double ch1 = tmp / h;
    size_t ch2 = tmp / h;
    double ch  = ch1 - (double) ch2;
    
    size_t mSize = (tmp / h) + 1;
    if(ch > 0) {
        ++mSize;
    }

    TMatrix Answer(2, mSize);
    Answer[0][0] = x0;
    Answer[1][0] = y0;

    double x = x0;
    double y = y0;
    double z = dy0;
    for(size_t pos = 1; pos < mSize; ++pos) {

        y += h * F(x, y, z);
        z += h * G(x, y, z);
        x += h;
        
        Answer[0][pos] = x;
        Answer[1][pos] = y;
    }

    return Answer;
}



TMatrix TrueAnswer(double (*Y)(double), double a, double b, double h) {
    double tmp = b - a;
    double ch1 = tmp / h;
    size_t ch2 = tmp / h;
    double ch  = ch1 - (double) ch2;
    
    size_t mSize = (tmp / h) + 1;
    if(ch > 0) {
        ++mSize;
    }

    TMatrix Answer(2, mSize);

    double x = a;
    double y;
    for(size_t pos = 0; pos < mSize; ++pos) {
        y = Y(x);

        Answer[0][pos] = x;
        Answer[1][pos] = y;
        
        x += h;
    }

    return Answer;
}


TMatrix RungeKutta(double (*F)(double, double, double),
                   double (*G)(double, double, double),
                   double x0, double y0, double dy0, double a, double b, double h) {
    
    double tmp = b - a;
    double ch1 = tmp / h;
    size_t ch2 = tmp / h;
    double ch  = ch1 - (double) ch2;
    
    size_t mSize = (tmp / h) + 1;
    if(ch > 0) {
        ++mSize;
    }

    TMatrix Answer(2, mSize);
    TMatrix K(4, 1), L(4, 1);
    Answer[0][0] = x0;
    Answer[1][0] = y0;

    double x = x0;
    double y = y0;
    double z = dy0;
    double deltaY, deltaZ;
    for(size_t pos = 1; pos < mSize; ++pos) {

        K[0][0] = h * F(x, y, z);
        L[0][0] = h * G(x, y, z);
        
        K[1][0] = h * F(x + (h / 2), y + (K[0][0] / 2), z + (L[0][0] / 2));
        L[1][0] = h * G(x + (h / 2), y + (K[0][0] / 2), z + (L[0][0] / 2));
        
        K[2][0] = h * F(x + (h / 2), y + (K[1][0] / 2), z + (L[1][0] / 2));
        L[2][0] = h * G(x + (h / 2), y + (K[1][0] / 2), z + (L[1][0] / 2));
        
        K[3][0] = h * F(x +  h     , y +  K[2][0]     , z +  L[2][0]     );
        L[3][0] = h * G(x +  h     , y +  K[2][0]     , z +  L[2][0]     );

        deltaY = (K[0][0] + 2 * K[1][0] + 2 * K[2][0] + K[3][0]) / 6;
        deltaZ = (L[0][0] + 2 * L[1][0] + 2 * L[2][0] + L[3][0]) / 6;

        y += deltaY;
        z += deltaZ;
        x += h;
        
        Answer[0][pos] = x;
        Answer[1][pos] = y;
    }

    return Answer;
}

TMatrix Adams(double (*F)(double, double, double),
              double (*G)(double, double, double),
              double x0, double y0, double dy0, double a, double b, double h) {
    double tmp = b - a;
    double ch1 = tmp / h;
    size_t ch2 = tmp / h;
    double ch  = ch1 - (double) ch2;
    
    size_t mSize = (tmp / h) + 1;
    if(ch > 0) {
        ++mSize;
    }

    TMatrix Answer(2, mSize);
    TMatrix Z(1, mSize);
    TMatrix K(4, 1), L(4, 1);
    
    Answer[0][0] = x0;
    Answer[1][0] = y0;
    Z[0][0]      = dy0;
    
    double x = x0;
    double y = y0;
    double z = dy0;
    double deltaY, deltaZ;
    
    for(size_t pos = 1; pos < 4; ++pos) {

        K[0][0] = h * F(x, y, z);
        L[0][0] = h * G(x, y, z);

        K[1][0] = h * F(x + (h / 2), y + (K[0][0] / 2), z + (L[0][0] / 2));
        L[1][0] = h * G(x + (h / 2), y + (K[0][0] / 2), z + (L[0][0] / 2));

        K[2][0] = h * F(x + (h / 2), y + (K[1][0] / 2), z + (L[1][0] / 2));
        L[2][0] = h * G(x + (h / 2), y + (K[1][0] / 2), z + (L[1][0] / 2));
        
        K[3][0] = h * F(x +  h     , y +  K[2][0]     , z +  L[2][0]     );
        L[3][0] = h * G(x +  h     , y +  K[2][0]     , z +  L[2][0]     );

        deltaY = (K[0][0] + 2 * K[1][0] + 2 * K[2][0] + K[3][0]) / 6;
        deltaZ = (L[0][0] + 2 * L[1][0] + 2 * L[2][0] + L[3][0]) / 6;

        y += deltaY;
        z += deltaZ;
        x += h;
        
        
        Answer[0][pos] = x;
        Answer[1][pos] = y;
        Z[0][pos]      = z;
    }

    for(size_t pos = 4; pos < mSize; ++pos) {

        y += (h / 24.0) * ( 55 * F(Answer[0][pos - 1], Answer[1][pos - 1], Z[0][pos - 1])
                          - 59 * F(Answer[0][pos - 2], Answer[1][pos - 2], Z[0][pos - 2])
                          + 37 * F(Answer[0][pos - 3], Answer[1][pos - 3], Z[0][pos - 3])
                          - 9  * F(Answer[0][pos - 4], Answer[1][pos - 4], Z[0][pos - 4]) );
        z += (h / 24.0) * ( 55 * G(Answer[0][pos - 1], Answer[1][pos - 1], Z[0][pos - 1])
                          - 59 * G(Answer[0][pos - 2], Answer[1][pos - 2], Z[0][pos - 2])
                          + 37 * G(Answer[0][pos - 3], Answer[1][pos - 3], Z[0][pos - 3])
                          - 9  * G(Answer[0][pos - 4], Answer[1][pos - 4], Z[0][pos - 4]) );
        x += h;
        
        Answer[0][pos] = x;
        Answer[1][pos] = y;
        Z[0][pos]      = z;
    }
    
    return Answer;
}


TMatrix Shooting(double (*F)(double, double, double),
                 double (*G)(double, double, double),
                 double (*C)(double, double, double),
                 double x0, double y0, double dy0, double a, double b, double h) {

    TMatrix Prev, Current;

    double x  = x0;
    double y  = y0;
    double z1 = dy0;
    double z2 = dy0 + 0.1;
    double tmp;
    Prev      = RungeKutta(F, G, x, y, z1, a, b, h);
    Current   = RungeKutta(F, G, x, y, z2, a, b, h);
    
    size_t rb  = Current.GetSB() - 1;

    
    double ch1 = C(Prev   [0][rb], Prev   [1][rb], z1);
    double ch2 = C(Current[0][rb], Current[1][rb], z2);
    
    while(((ch2 > 0) ? ch2 : -ch2) > ACC_RATE) {

        tmp = z2;
        z2 -=  ch2 * ((z2 - z1) / (ch2 - ch1));
        z1 = tmp;
        
        Prev      = Current;
        Current   = RungeKutta(F, G, x, y, z2, a, b);

        ch1 = ch2;
        ch2 = C(Current[0][rb], Current[1][rb], z2);
    }

    return Current;
}


TMatrix FinDiffMethod(double (*F)(double, double, double), double (*G)(double, double, double), double x0, double y0, double dy0, double a, double b, double h) {

    double x = x0;

    
    TMatrix Answer(2, S_TABLE);

    size_t NLines = S_TABLE - 1;
    double stepX = (b - a) / NLines;
    for(size_t j = 0; j < S_TABLE; ++j) {
        Answer[0][j] = x;
        x += stepX;
    }

    TMatrix A(S_TABLE, S_TABLE), B(S_TABLE, 1);

    A[0][0] = -1.0 / stepX;
    A[0][1] = 1.0 / stepX;
    B[0][0] = 0;
    
    for(size_t i = 1; i < (S_TABLE - 1); ++i) {
        A[i][i - 1] = 1 - (PFunc(Answer[0][i]) * stepX / 2.0);
        A[i][i]     = -2 + h * h * QFunc(Answer[0][i]);
        A[i][i + 1] = 1 + (PFunc(Answer[0][i]) * stepX / 2.0);
        B[i][0]     = stepX * stepX * FFunc(Answer[0][i]);
    }
    
    A[NLines][NLines - 1] = -1.0 / stepX;
    A[NLines][NLines]     = 1.5 + (1.0 / stepX);
    B[NLines][0]          = exp(2.0);
    
    TMatrix Y = SweepMethod(A, B);

    for(size_t j = 0; j < S_TABLE; ++j) {
        Answer[1][j] = Y[j][0];
    }
    return Answer;
}
