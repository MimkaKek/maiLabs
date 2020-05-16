#include <iostream>
#include "../Lib/globals.hpp"
#include "../Lib/matrix.hpp"
#include "../Lib/functions.hpp"
#include "../Lib/aproxcalc.hpp"

int main() {

    double x0, a, b, x;
    std::cout << "=======================" << std::endl;
    std::cout << "|       PART ONE      |" << std::endl;
    std::cout << "=======================" << std::endl;
    std::cout << "|    NEWTON METHOD    |" << std::endl;
    std::cout << "=======================" << std::endl;
    std::cin >> x0;
    std::cin >> a;
    std::cin >> b;
    x = NewtonMethod(a, b, x0, EPSILON);
    std::cout << x << std::endl;
    std::cout <<"Check:" << std::endl;
    std::cout << "F(X) = " << Func(x) << std::endl;
    std::cout << "=======================" << std::endl;
    std::cout << "|  SIMPLE ITERATIONS  |" << std::endl;
    std::cout << "=======================" << std::endl;
    std::cin >> x0;
    std::cin >> a;
    std::cin >> b;
    x = SimpleIterations(a, b, x0, EPSILON);
    std::cout << x << std::endl;
    std::cout <<"Check:" << std::endl;
    std::cout << "F(X) = " << Func(x) << std::endl;
    std::cout << "=======================" << std::endl;
    std::cout << "|       PART TWO      |" << std::endl;
    std::cout << "=======================" << std::endl;
    std::cout << "|    NEWTON METHOD    |" << std::endl;
    std::cout << "=======================" << std::endl;
    TMatrix X0(std::cin);
    TMatrix X;
    X = NewtonMethod(X0); // 3.5 1
    std::cout << "X: " << std::endl;
    std::cout << X;
    std::cout <<"Check:" << std::endl;
    std::cout << "/ F1(X) = " << SF1(X[0][0], X[1][0]) << std::endl;
    std::cout << "\\ F2(X) = " << SF2(X[0][0], X[1][0]) << std::endl;
    std::cout << "=======================" << std::endl;
    std::cout << "|  SIMPLE ITERATIONS  |" << std::endl;
    std::cout << "=======================" << std::endl;
    TMatrix A(std::cin);
    TMatrix B(std::cin);
    X0 = TMatrix(std::cin);
    X = SimpleIterations(X0, A, B);
    std::cout << "X: " << std::endl;
    std::cout << X;
    std::cout <<"Check:" << std::endl;
    std::cout << "/ F1(X) = " << SF1(X[0][0], X[1][0]) << std::endl;
    std::cout << "\\ F2(X) = " << SF2(X[0][0], X[1][0]) << std::endl;
    return 0;
}
