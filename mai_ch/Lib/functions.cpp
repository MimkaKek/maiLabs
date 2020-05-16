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
    return (x1*x1 - 3 + (x2 - 1)*(x2 - 1))/2;
}

double dSFi11(double x1, double x2) {
    return x1;
}

double dSFi12(double x1, double x2) {
    return x2 - 0.5;
}

//====================================================

double SFi2(double x1, double x2) {
    return 8/(x1*x1 + 4);
}

double dSFi21(double x1, double x2) {
    return (-16*x1)/((x1*x1 + 4)*(x1*x1 + 4));
}

double dSFi22(double x1, double x2) {
    return 0;
}
