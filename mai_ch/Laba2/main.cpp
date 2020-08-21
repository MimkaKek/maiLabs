#include <iostream>
#include "../Lib/globals.hpp"
#include "../Lib/matrix.hpp"
#include "../Lib/functions.hpp"
#include "../Lib/aproxcalc.hpp"

void PrintMenu(unsigned char type) {
    std::cout <<"====================" << std::endl;
    if(type == BEGIN_MENU) {
        std::cout << "Select part" << std::endl;
        std::cout <<"====================" << std::endl;
        std::cout << "1. One equation" << std::endl;
        std::cout << "2. System of equations" << std::endl;
        std::cout << "q. Exit from program" << std::endl;
    }
    else {
        std::cout << "Select method" << std::endl;
        std::cout <<"====================" << std::endl;
        std::cout << "1. Simple iterations" << std::endl;
        std::cout << "2. Newton method" << std::endl;
        std::cout << "b. Back" << std::endl;
        std::cout << "q. Exit from program" << std::endl;
    }
    std::cout <<"====================" << std::endl;
    return;
}

int main() {

    unsigned char   input;
    bool            isRun = true, inTask = false;
    bool            isSys = false;
    double          x0, a, b, x;
    
    while(isRun) {
        PrintMenu(BEGIN_MENU);
        std::cout << "> ";
        std::cin >> input;
        switch(input) {
            case '1':
            {
                isSys = false;
                inTask = true;
                break;
            }
            case '2':
            {
                isSys = true;
                inTask = true;
                break;
            }
            case 'q':
            {
                isRun = false;
                break;
            }
            default:
            {
                std::cout << "Error: Wrong command!" << std::endl;
                break;
            }
        }
        while(isRun && inTask) {
            PrintMenu(TASK_MENU);
            std::cout << "> ";
            std::cin >> input;
            switch(input) {
                case '1':
                {
                    if(isSys) {
                        std::cout << "Input A:" << std::endl;
                        TMatrix A(2, 1);
                        std::cin >> A[0][0];
                        std::cin >> A[1][0];
                        std::cout << "Input B:" << std::endl;
                        TMatrix B(2, 1);
                        std::cin >> B[0][0];
                        std::cin >> B[1][0];
                        std::cout << "Input x0:" << std::endl;
                        TMatrix X0(2, 1);
                        std::cin >> X0[0][0];
                        std::cin >> X0[1][0];
                        TMatrix X;
                        X = SimpleIterations(X0, A, B); 
                        std::cout << "Result: " << std::endl;
                        std::cout << X;
                        std::cout <<"Check:" << std::endl;
                        std::cout << "/ F1(X) = " << SF1(X[0][0], X[1][0]) << std::endl;
                        std::cout << "\\ F2(X) = " << SF2(X[0][0], X[1][0]) << std::endl;
                    }
                    else {
                        std::cout << "Recommended input:" << std::endl;
                        std::cout << "x0 = 5, a = 4, b = 6" << std::endl;
                        std::cout << "==========================" << std::endl;
                        std::cout << "Input x0: ";
                        std::cin >> x0;
                        std::cout << "Input a: ";
                        std::cin >> a;
                        std::cout << "Input b: ";
                        std::cin >> b;
                        x = SimpleIterations(a, b, x0, EPSILON);
                        std::cout << "Result: " << x << std::endl;
                        std::cout << "Check:" << std::endl;
                        std::cout << "  F(" << x << ") = " << Func(x) << std::endl;
                    }
                    break;
                }
                case '2':
                {
                    if(isSys) {
                        std::cout << "Input x0: ";
                        TMatrix X0(2, 1);
                        std::cin >> X0[0][0];
                        std::cin >> X0[1][0]; // 2 1 3.5 1
                        TMatrix X;
                        X = NewtonMethod(X0);
                        std::cout << "Result: " << std::endl;
                        std::cout << X;
                        std::cout << "Check:" << std::endl;
                        std::cout << "/ F1(X) = " << SF1(X[0][0], X[1][0]) << std::endl;
                        std::cout << "\\ F2(X) = " << SF2(X[0][0], X[1][0]) << std::endl;
                    }
                    else {
                        std::cout << "Recommended inputs:" << std::endl;
                        std::cout << "1) x0 = 1, a = 0.5, b = 2" << std::endl;
                        std::cout << "2) x0 = 5, a = 4, b = 6" << std::endl;
                        std::cout << "==========================" << std::endl;
                        std::cout << "Input x0: ";
                        std::cin >> x0;
                        std::cout << "Input a: ";
                        std::cin >> a;
                        std::cout << "Input b: ";
                        std::cin >> b;
                        x = NewtonMethod(a, b, x0, EPSILON);
                        std::cout << x << std::endl;
                        std::cout <<"Check:" << std::endl;
                        std::cout << "F(X) = " << Func(x) << std::endl;
                    }
                    break;
                }
                case 'b':
                {
                    inTask = false;
                    break;
                }
                case 'q':
                {
                    isRun = false;
                    break;
                }
                default:
                    std::cout << "Error: Bad command!" << std::endl;
            }
        }
    }
    return 0;
}
