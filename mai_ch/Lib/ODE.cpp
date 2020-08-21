#include "ODE.hpp"

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
    double z2 = dy0 - 0.2;
    double tmp;
    Prev      = RungeKutta(F, G, x, y, z1, a, b);
    Current   = RungeKutta(F, G, x, y, z2, a, b);
    
    size_t s  = Current.GetSB();

    double ch2 = C(Current[0][s - 1], Current[1][s - 1], z2);
    double ch1 = C(Current[0][s - 1], Current[1][s - 1], z1);
    
    while(((ch2 > 0) ? ch2 : -ch2) > ACC_RATE) {

        tmp = z2;
        z2 = z2 - ((z2 - z1) * ch2) / (ch2 - ch1);
        z1 = tmp;
        
        Prev      = RungeKutta(F, G, x, y, z1, a, b);
        Current   = RungeKutta(F, G, x, y, z2, a, b);

        ch1 = ch2;
        ch2 = C(Current[0][s - 1], Current[1][s - 1], z2);
    }

    return Current;
}
