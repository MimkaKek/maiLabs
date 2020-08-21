#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <cmath>

double Func(double x);
double dFunc(double x);
double ddFunc(double x);

double Fi1(double x);
double dFi1(double x);

double Fi2(double x);
double dFi2(double x);

double SF1(double x1, double x2);
double dSF11(double x1, double x2);
double dSF12(double x1, double x2);

double SF2(double x1, double x2);
double dSF21(double x1, double x2);
double dSF22(double x1, double x2);

double SFi1(double x1, double x2);
double dSFi11(double x1, double x2);
double dSFi12(double x1, double x2);

double SFi2(double x1, double x2);
double dSFi21(double x1, double x2);
double dSFi22(double x1, double x2);

double G1(double x, double y, double z);
double F1(double x, double y, double z);

double G2(double x, double y, double z);
double F2(double x, double y, double z);

double TrueODE1(double x);
double TrueODE2(double x);

double CheckFunc(double x, double y, double z);
#endif
