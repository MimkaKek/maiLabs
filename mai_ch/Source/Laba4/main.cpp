#include <iostream>
#include <fstream>
#include <string>
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
        std::cout << "4. Draw via gnuplot" << std::endl;
        std::cout << "b. Back" << std::endl;
        std::cout << "q. Exit from program" << std::endl;
    }
    else {
        std::cout << "Task table" << std::endl;
        std::cout <<"====================" << std::endl;
        std::cout << "1. Shooting method" << std::endl;
        std::cout << "2. Finite difference method" << std::endl;
        std::cout << "3. Draw via gnuplot" << std::endl;
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
                        TMatrix answer = Shooting(F2, G2, CheckFunc, 1.0, exp(1), 0.0, 1.0, 2.0, 0.1);
                        TMatrix check = TrueAnswer(TrueODE2, 1.0, 2.0, 0.1);
                        std::cout << "Shooting method: " << std::endl;
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
                        TMatrix answer = FinDiffMethod(F2, G2, 1.0, exp(1), 0.0, 1.0, 2.0, 0.1);
                        TMatrix check = TrueAnswer(TrueODE2, 1.0, 2.0, 0.1);
                        std::cout << "Finite difference method: " << std::endl;
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
                        TMatrix A1 = Shooting(F2, G2, CheckFunc, 1.0, exp(1), 0.0, 1.0, 2.0, 0.1);
                        TMatrix A2 = FinDiffMethod(F2, G2, 1.0, exp(1), 0.0, 1.0, 2.0, 0.1);
                        TMatrix AT = TrueAnswer(TrueODE2, 1.0, 2.0, 0.1);

                        std::ofstream file("Shooting");
                        for (int i = 0; i < A1.GetSB(); ++i) { 
                            file << A1[0][i] << " " << A1[1][i] << "\n";
                        }
                        file.close();
                        
                        std::ofstream file1("FinDiffMethod");
                        for (int i = 0; i < A2.GetSB(); ++i) { 
                            file1 << A2[0][i] << " " << A2[1][i] << "\n";
                        }
                        file1.close();

                        std::ofstream file3("Accurate");
                        for (int i = 0; i < AT.GetSB(); ++i) {
                            file3 << AT[0][i] << " " << AT[1][i] << "\n";
                        }
                        file3.close();
                        FILE* gp = popen("gnuplot -persist", "w");
                        if (gp == NULL) {
                            std::cout << "Error: gnuplot didn't open\n";
                            return 1;
                        }
                        std::string gnuplot = "plot \"Shooting\" with lines, \"FinDiffMethod\" with lines, \"Accurate\" with lines\n";
                        fprintf(gp, gnuplot.c_str());
                        pclose(gp);
                    }
                    break;
                }
                case '4':
                {
                    if(part == 1) {
                        TMatrix A1 = Euler(F1, G1, 0.0, 1.0, 1.0, 0.0, 1.0, 0.1);
                        TMatrix A2 = RungeKutta(F1, G1, 0.0, 1.0, 1.0, 0.0, 1.0, 0.1);
                        TMatrix A3 = Adams(F1, G1, 0.0, 1.0, 1.0, 0.0, 1.0, 0.1);
                        TMatrix AT = TrueAnswer(TrueODE1, 0.0, 1.0, 0.1);
                        
                        std::ofstream file("Euler");
                        for (int i = 0; i < A1.GetSB(); ++i) { 
                            file << A1[0][i] << " " << A1[1][i] << "\n";
                        }
                        file.close();
                        
                        std::ofstream file1("Runge");
                        for (int i = 0; i < A2.GetSB(); ++i) { 
                            file1 << A2[0][i] << " " << A2[1][i] << "\n";
                        }
                        file1.close();
                        
                        std::ofstream file2("Adams");
                        for (int i = 0; i < A3.GetSB(); ++i) { 
                            file2 << A3[0][i] << " " << A3[1][i] << "\n";
                        }
                        file2.close();

                        std::ofstream file3("Accurate");
                        for (int i = 0; i < AT.GetSB(); ++i) {
                            file3 << AT[0][i] << " " << AT[1][i] << "\n";
                        }
                        file3.close();
                        FILE* gp = popen("gnuplot -persist", "w");
                        if (gp == NULL) {
                            std::cout << "Error: gnuplot didn't open\n";
                            return 1;
                        }
                        std::string gnuplot = "plot \"Euler\" with lines, \"Runge\" with lines, \"Adams\" with lines, \"Accurate\" with lines\n";
                        fprintf(gp, gnuplot.c_str());
                        pclose(gp);
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
