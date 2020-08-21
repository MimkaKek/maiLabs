#include <iostream>
#include <vector>
#include <cmath>
#include "../Lib/globals.hpp"
#include "../Lib/functions.hpp"
#include "../Lib/matrix.hpp"
#include "../Lib/ODE.hpp"

bool IS_TEST = true;

void PrintMenu(unsigned char type) {
    std::cout <<"====================" << std::endl;
    
    if(type == BEGIN_MENU) {
        std::cout << "Main menu" << std::endl;
        std::cout <<"====================" << std::endl;
        std::cout << "1. Part 1" << std::endl;
        std::cout << "2. Part 2" << std::endl;
        std::cout << "q. Exit from program" << std::endl;
    }
    else if(type == PART1_MENU) {
        std::cout << "Task table" << std::endl;
        std::cout <<"====================" << std::endl;
        std::cout << "1. Euler" << std::endl;
        std::cout << "2. Runge Kutta" << std::endl;
        std::cout << "3. Adams" << std::endl;
        std::cout << "b. Back" << std::endl;
        std::cout << "q. Exit from program" << std::endl;
    }
    else {
        std::cout << "Task table" << std::endl;
        std::cout <<"====================" << std::endl;
        std::cout << "1. Shooting method" << std::endl;
        std::cout << "2. Finite difference method" << std::endl;
        std::cout << "b. Back" << std::endl;
        std::cout << "q. Exit from program" << std::endl;
    }

    std::cout <<"====================" << std::endl;
    return;
}

int main() {

    unsigned char   input;
    bool            isRun = true, inTask = false;
    unsigned char   part = 0;

    while(isRun) {
        PrintMenu(BEGIN_MENU);
        std::cout << "> ";
        std::cin >> input;
        switch(input) {
            case '1':
            {
                part = 1;
                inTask = true;
                break;
            }
            case '2':
            {
                part = 2;
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
            if(part == 1) {
                PrintMenu(PART1_MENU);
            }
            else {
                PrintMenu(PART2_MENU);
            }
            
            std::cout << "> ";
            std::cin >> input;
            switch(input) {
                case '1':
                {
                    if(part == 1) {
                        TMatrix answer = Euler(F1, G1, 0.0, 1.0, 1.0, 0.0, 1.0, 0.1);
                        TMatrix check = TrueAnswer(TrueODE1, 0.0, 1.0, 0.1);
                        std::cout << "Euler: " << std::endl;
                        std::cout << answer;
                        std::cout << "================" << std::endl;
                        std::cout << "Currect Answer: " << std::endl;
                        std::cout << check;
                        std::cout << "================" << std::endl;
                        std::cout << "Diff: " << std::endl;
                        std::cout << check - answer;
                    }
                    else {
                        TMatrix answer = Shooting(F2, G2, CheckFunc, 1.0, 0.0, 1.0, 1.0, 2.0, 0.1);
                        TMatrix check = TrueAnswer(TrueODE2, 1.0, 2.0, 0.1);
                        std::cout << "Euler: " << std::endl;
                        std::cout << answer;
                        std::cout << "================" << std::endl;
                        std::cout << "Currect Answer: " << std::endl;
                        std::cout << check;
                        std::cout << "================" << std::endl;
                        std::cout << "Diff: " << std::endl;
                        std::cout << check - answer;
                    }
                    break;
                }
                case '2':
                {
                    if(part == 1) {
                        TMatrix answer = RungeKutta(F1, G1, 0.0, 1.0, 1.0, 0.0, 1.0, 0.1);
                        TMatrix check = TrueAnswer(TrueODE1, 0.0, 1.0, 0.1);
                        std::cout << "Runge–Kutta method: " << std::endl;
                        std::cout << answer;
                        std::cout << "================" << std::endl;
                        std::cout << "Currect Answer: " << std::endl;
                        std::cout << check;
                        std::cout << "================" << std::endl;
                        std::cout << "Diff: " << std::endl;
                        std::cout << check - answer;
                    }
                    else {
                        
                    }
                    
                    break;
                }
                case '3':
                {
                    if(part == 1) {
                        TMatrix answer = Adams(F1, G1, 0.0, 1.0, 1.0, 0.0, 1.0, 0.1);
                        TMatrix check = TrueAnswer(TrueODE1, 0.0, 1.0, 0.1);
                        std::cout << "Adams method: " << std::endl;
                        std::cout << answer;
                        std::cout << "================" << std::endl;
                        std::cout << "Currect Answer: " << std::endl;
                        std::cout << check;
                        std::cout << "================" << std::endl;
                        std::cout << "Diff: " << std::endl;
                        std::cout << check - answer;
                    }
                    else {
                        std::cout << "Error: Bad command!" << std::endl;
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
