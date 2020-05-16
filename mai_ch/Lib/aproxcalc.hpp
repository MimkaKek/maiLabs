#ifndef APROXCALC_HPP
#define APROXCALC_HPP

#include "globals.hpp"
#include "matrix.hpp"
#include "functions.hpp"

TMatrix NewtonMethod(TMatrix& X0, double eps = EPSILON);
TMatrix SimpleIterations(TMatrix& X0, TMatrix A, TMatrix B, double eps = EPSILON);
double NewtonMethod(double a, double b, double begin, double eps = EPSILON);
double SimpleIterations(double a, double b, double begin, double eps = EPSILON);

#endif
