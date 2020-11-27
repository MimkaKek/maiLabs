#include <iostream>
#include <fstream>
#include <string>
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
        std::cout << "4. MinSquare" << std::endl;
        std::cout << "5. Derivatives" << std::endl;
        std::cout << "6. Integral" << std::endl;
        std::cout << "7. Draw MinSquare via gnuplot" << std::endl;
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
    TMinSquare      mSquare1(&table, 1);
    TMinSquare      mSquare2(&table, 2);
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
                    x = 3.14 / 4.0;
                    y = lInter(x) - sin(x);
                    std::cout << "Diff in x* = pi / 4 equal " << (y > 0 ? y : -y) << std::endl;
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
                    x = 3.14 / 4.0;
                    y = nInter(x) - sin(x);
                    std::cout << "Diff in x* = pi / 4 equal " <<  (y > 0 ? y : -y) << std::endl;
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
                    if(changeTable[2]) {
                        mSquare1.Update();
                        mSquare2.Update();
                        changeTable[2] = false;
                    }
                    std::cout << "Input x: ";
                    std::cin >> x;
                    std::cout << "MinSquare_1(X) = " << mSquare1(x) << std::endl;
                    mSquare1.Errors();
                    std::cout << "MinSquare_2(X) = " << mSquare2(x) << std::endl;
                    mSquare2.Errors();
                    break;
                }
                case '5':
                {
                    std::cout << "Input x: ";
                    std::cin >> x;
                    Derivative(&table, x);
                    break;
                }
                case '6':
                {
                    Integral(iFunc, -1, 1, 0.5, 0.25);
                    break;
                }
                case '7':
                {
                    mSquare1.Update();
                    mSquare2.Update();
                    double x = -1.0;
                    std::ofstream file("ApproxPol_1");
                    for (int i = -1; i < 5; ++i) {
                        file << x << " " << mSquare1(x) << "\n";
                        x += 1.0;
                    }
                    file.close();

                    x = -1.0;
                    std::ofstream file1("ApproxPol_2");
                    for (int i = -1; i < 5; ++i) { 
                        file1 << x << " " << mSquare2(x) << "\n";
                        x += 1.0;
                    }
                    file1.close();

                    std::ofstream file3("Accurate");
                    for (int i = 1; i <= table.getSize(); ++i) {
                        file3 << table[i].first << " " << table[i].second << "\n";
                    }
                    file3.close();
                    FILE* gp = popen("gnuplot -persist", "w");
                    if (gp == NULL) {
                        std::cout << "Error: gnuplot didn't open\n";
                        return 1;
                    }
                    std::string gnuplot = "plot \"ApproxPol_1\" with lines, \"ApproxPol_2\" with lines, \"Accurate\" with lines\n";
                    fprintf(gp, gnuplot.c_str());
                    pclose(gp);
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
