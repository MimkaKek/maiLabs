#ifndef ODE_HPP
#define ODE_HPP

#include "matrix.hpp"

double ACC_RATE = 0.0001;

TMatrix Euler(double (*F)(double, double, double), double (*G)(double, double, double), double x0, double y0, double dy0, double a, double b, double h = 0.1);

TMatrix RungeKutta(double (*F)(double, double, double), double (*G)(double, double, double), double x0, double y0, double dy0, double a, double b, double h = 0.1);

TMatrix Adams(double (*F)(double, double, double), double (*G)(double, double, double), double x0, double y0, double dy0, double a, double b, double h = 0.1);

TMatrix Shooting(double (*F)(double, double, double), double (*G)(double, double, double), double (*C)(double, double, double), double x0, double y0, double dy0, double a, double b, double h = 0.1);

TMatrix TrueAnswer(double (*Y)(double), double a, double b, double h = 0.1);

#endif
