#include <iostream>
#include <vector>
#include <cmath>
#include "../Lib/interpolation.hpp"
#include "../Lib/globals.hpp"
#include "../Lib/matrix.hpp"

bool IS_TEST = true;

double Func(double x) {
    return sin(x);
}

double iFunc(double x) {
    return x / (2*x + 5);
}

void PrintMenu(unsigned char type) {
    std::cout <<"====================" << std::endl;
    if(type == BEGIN_MENU) {
        std::cout << "Main menu" << std::endl;
        std::cout <<"====================" << std::endl;
        std::cout << "1. Add point" << std::endl;
        std::cout << "2. Remove point" << std::endl;
        std::cout << "3. Show table of points" << std::endl;
        std::cout << "4. Functions" << std::endl;
        std::cout << "q. Exit from program" << std::endl;
    }
    else {
        std::cout << "Task table" << std::endl;
        std::cout <<"====================" << std::endl;
        std::cout << "1. Lagrange interpolation" << std::endl;
        std::cout << "2. Newton interpolation" << std::endl;
        std::cout << "3. Splines" << std::endl;
        std::cout << "4. Derivatives" << std::endl;
        std::cout << "5. Integral" << std::endl;
        std::cout << "b. Back" << std::endl;
        std::cout << "q. Exit from program" << std::endl;
    }
    std::cout <<"====================" << std::endl;
    return;
}

int main() {

    TTableOfPoints  table;
    TLagrangeInter  lInter(&table);
    TNewtonInter    nInter(&table);
    TSplines        spl(&table);
    unsigned char   input;
    bool            isRun = true, inTask = false;
    bool            changeTable[3] = {false, false, false};
    double          x, y;
    size_t          n;

    while(isRun) {
        PrintMenu(BEGIN_MENU);
        std::cout << "> ";
        std::cin >> input;
        switch(input) {
            case '1':
            {
                if(IS_TEST) {
                    std::cout << "Input x: ";
                    std::cin >> x;
                    std::cout << "Input y: ";
                    std::cin >> y;
                }
                else {
                    std::cout << "Input x: ";
                    std::cin >> x;
                    y = Func(x);
                }
                
                table.AddPoint(x, y);
                changeTable[0] = true;
                changeTable[1] = true;
                changeTable[2] = true;
                break;
            }
            case '2':
            {
                if(table.IsEmpty()) {
                    std::cout << "Table already empty" << std::endl;
                    break;
                }
                std::cout << table;
                std::cout << "Input number of point: ";
                std::cin >> n;
                table.RemovePoint(n);
                changeTable[0] = true;
                changeTable[1] = true;
                changeTable[2] = true;
                break;
            }
            case '3':
            {
                table.PrintTable();
                break;
            }
            case '4':
            {
                inTask = true;
                break;
            }
            case 't':
            {
                std::cout << "Number: ";
                std::cin >> n;
                std::cout << table[n].first << " : " << table[n].second << std::endl;
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
                    if(changeTable[0]) {
                        lInter.Update();
                        changeTable[0] = false;
                    }
                    std::cout << "Input x: ";
                    std::cin >> x;
                    y = lInter(x);
                    std::cout << "L(X) = " << y << std::endl;
                    break;
                }
                case '2':
                {
                    if(changeTable[1]) {
                        nInter.Update();
                        changeTable[1] = false;
                    }
                    std::cout << "Input x: ";
                    std::cin >> x;
                    y = nInter(x);
                    std::cout << "N(X) = " << y << std::endl;
                    break;
                }
                case '3':
                {
                    if(changeTable[2]) {
                        spl.Update();
                        changeTable[2] = false;
                    }
                    std::cout << "Input x: ";
                    std::cin >> x;
                    y = spl(x);
                    std::cout << "S(X) = " << y << std::endl;
                    break;
                }
                case '4':
                {
                    std::cout << "Input x: ";
                    std::cin >> x;
                    Derivative(&table, x);
                    break;
                }
                case '5':
                {
                    Integral(iFunc, -1, 1, 0.5, 0.25);
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
