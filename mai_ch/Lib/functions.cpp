#include "functions.hpp"

//====================================================

double Func(double x) {
    return pow(2, x) - pow(x, 2) - 0.5;
}

double dFunc(double x) {
    return log(2) * pow(2, x) - 2*x;
}

double ddFunc(double x) {
    return log(2) * log(2) * pow(2, x) - 2;
}

//====================================================

double Fi1(double x) {
    return sqrt(pow(2, x) - 0.5);
}

double dFi1(double x) {
    return (log(2)*pow(2, x)) / (sqrt(pow(2, x) - 0.5));
}

//====================================================

double Fi2(double x) {
    return log10(x * x + 0.5)/log10(2);
}

double dFi2(double x) {
    return 2*x/(log10(2)*log(10)*(x * x + 0.5));
}

//====================================================

double SF1(double x1, double x2) {
    return (x1*x1 + 4)*x2 - 8;
}

double dSF11(double x1, double x2) {
    return 2*x1*x2;
}

double dSF12(double x1, double x2) {
    return x1*x1 + 4;
}

//====================================================

double SF2(double x1, double x2) {
    return (x1 - 1)*(x1 - 1) + (x2 - 1)*(x2 - 1) - 4;
}

double dSF21(double x1, double x2) {
    return 2*(x1 - 1);
}

double dSF22(double x1, double x2) {
    return 2*(x2 - 1);
}

//====================================================

double SFi1(double x1, double x2) {
    return sqrt((8 / x2) - 4);
}

double dSFi11(double x1, double x2) {
    return 0;
}

double dSFi12(double x1, double x2) {
    return (-8.0) / (sqrt((8.0 / pow(x2, 2.0)) - 4.0) * pow(x2, 3.0));
}

//====================================================

double SFi2(double x1, double x2) {
    return sqrt(4 - pow((x1 - 1), 2)) + 1;
}

double dSFi21(double x1, double x2) {
    return (x1 - 1) / sqrt(4 - pow((x1 - 1), 2));
}

double dSFi22(double x1, double x2) {
    return 0;
}

//====================================================

double G1(double x, double y, double z) {
    return sin(3.0 * x) - y;
}

double F1(double x, double y, double z) {
    return z;
}

double G2(double x, double y, double z) {
    return y - (2.0 * z) / x;
}

double F2(double x, double y, double z) {
    return z;
}

double TrueODE1(double x) {
    return cos(x) + (11.0/8.0)*sin(x) - sin(3*x)/8.0;
}

double TrueODE2(double x) {
    return exp(x) / x;
}

double CheckFunc(double x, double y, double z) {
    return 1.5 * y + z - exp(2.0);
}
//====================================================
