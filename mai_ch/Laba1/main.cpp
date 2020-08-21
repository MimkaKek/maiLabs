#include <iostream>
#include "../Lib/matrix.hpp"

void PrintMenu(unsigned char type) {
    std::cout <<"====================" << std::endl;
    if(type == BEGIN_MENU) {
        std::cout << "Main menu" << std::endl;
        std::cout <<"====================" << std::endl;
        std::cout << "1. Input matrix A" << std::endl;
        std::cout << "2. Input vector B" << std::endl;
        std::cout << "3. Functions" << std::endl;
        std::cout << "q. Exit from program" << std::endl;
    }
    else {
        std::cout << "Task table" << std::endl;
        std::cout <<"====================" << std::endl;
        std::cout << "1. LU" << std::endl;
        std::cout << "2. Sweep Method" << std::endl;
        std::cout << "3. Iterations" << std::endl;
        std::cout << "4. Eigenvector and Eigenvalue" << std::endl;
        std::cout << "5. QR" << std::endl;
        std::cout << "b. Back" << std::endl;
        std::cout << "q. Exit from program" << std::endl;
    }
    std::cout <<"====================" << std::endl;
    return;
}

int main() {

    unsigned char   input, cont;
    bool            isRun = true, inTask = false;
    bool            matrices[2] = {false, false};
    TMatrix A, B;
    
    while(isRun) {
        PrintMenu(BEGIN_MENU);
        std::cout << "> ";
        std::cin >> input;
        switch(input) {
            case '1':
            {
                A = TMatrix(std::cin);
                std::cout << "Your matrix A:" << std::endl;
                std::cout << A;
                matrices[0] = true;
                break;
            }
            case '2':
            {
                B = TMatrix(std::cin);
                std::cout << "Your vector B:" << std::endl;
                std::cout << B;
                matrices[1] = true;
                break;
            }
            case '3':
            {
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
                    if(!matrices[0] || !matrices[1]) {
                        std::cout << "First you need to input:" << std::endl;
                        if(!matrices[0]) {
                            std::cout << "- Matrix A" << std::endl;
                        }
                        if(!matrices[1]) {
                            std::cout << "- Vector B" << std::endl;
                        }
                        std::cin >> input;
                        inTask = false;
                        break;
                    }

                    TMatrix X;
                    std::pair<TMatrix, TMatrix> LUB;
                    LUB = LUDecomposition(A, B);
                    std::cout << "Matrix LU:" << std::endl;
                    std::cout << LUB.first;
                    std::cout << "Vector B:" << std::endl;
                    std::cout << LUB.second;
                    X = SolveSystemViaLU(LUB.first, LUB.second);
                    std::cout << "Vector X:" << std::endl;
                    std::cout << X;
    
                    std::cout << "Check? ";
                    std::cin >> cont;
                    if(cont == 'y') {
                        std::cout << "Vector B: " << std::endl;
                        std::cout << B;
                        std::cout << "Vector A*X: " << std::endl;
                        std::cout << A*X;
                    }

                    std::cout << "Calculate determinant..." << std::endl;
                    std::cout << "Det(A) = " << GetDeterminant(LUB.first) << " via LU" << std::endl;

                    std::cout << "Check? ";
                    std::cin >> cont;
                    if(cont == 'y') {
                        std::cout << "Det(A) = " << A.GetDeterminant() << "via classic method" << std::endl;
                    }
                    
                    std::cout << "Inverse Matrix..." << std::endl;
                    TMatrix InvA = InverseMatrix(A);
    
                    std::cout << "A^(-1):" << std::endl;
                    std::cout << InvA;
    
                    std::cout << "Check? ";
                    std::cin >> cont;
                    if(cont == 'y') {
                        std::cout << "Matrix A*A^(-1): " << std::endl;
                        std::cout << A*InvA;
                    }
                    std::cout << "End!" << std::endl;
                    break;
                }
                case '2':
                {
                    if(!matrices[0] || !matrices[1]) {
                        std::cout << "First you need to input:" << std::endl;
                        if(!matrices[0]) {
                            std::cout << "- Matrix A" << std::endl;
                        }
                        if(!matrices[1]) {
                            std::cout << "- Vector B" << std::endl;
                        }
                        inTask = false;
                        break;
                    }
                    
                    TMatrix X = SweepMethod(A, B);
                    std::cout << "Vector X:" << std::endl;
                    std::cout << X;
    
                    std::cout << "Check? ";
                    std::cin >> cont;
                    if(cont == 'y') {
                        std::cout << "Vector B: " << std::endl;
                        std::cout << B;
                        std::cout << "Vector A*X: " << std::endl;
                        std::cout << A*X;
                    }
                    std::cout << "End!" << std::endl;
                    break;
                }
                case '3':
                {
                    if(!matrices[0] || !matrices[1]) {
                        std::cout << "First you need to input:" << std::endl;
                        if(!matrices[0]) {
                            std::cout << "- Matrix A" << std::endl;
                        }
                        if(!matrices[1]) {
                            std::cout << "- Vector B" << std::endl;
                        }
                        inTask = false;
                        break;
                    }

                    std::cout << "Calculate X via Simple Iterations..." << std::endl;
                    TMatrix X = SimpleIterations(A, B);
                    std::cout << "Vector X:" << std::endl;
                    std::cout << X;
    
                    std::cout << "Check? - ";
                    std::cin >> cont;
                    if(cont == 'y') {
                        std::cout << "Vector B: " << std::endl;
                        std::cout << B;
                        std::cout << "Vector A*X: " << std::endl;
                        std::cout << A*X;
                    }
    
                    std::cout << "Calculate X via Seidel Method..." << std::endl;
                    X = SeidelMethod(A, B);
                    std::cout << "Vector X:" << std::endl;
                    std::cout << X;
    
                    std::cout << "Check? - ";
                    std::cin >> cont;
                    if(cont == 'y') {
                        std::cout << "Vector B: " << std::endl;
                        std::cout << B;
                        std::cout << "Vector A*X: " << std::endl;
                        std::cout << A*X;
                    }
                    std::cout << "End!" << std::endl;
                    break;
                }
                case '4':
                {
                    if(!matrices[0]) {
                        std::cout << "First you need to input:" << std::endl;
                        std::cout << "- Matrix A" << std::endl;
                        inTask = false;
                        break;
                    }
                    
                    double eps;
                    size_t sizeM;
                    std::cout << "Input epsilon: ";
                    std::cin >> eps;
                    
                    std::pair<TMatrix, TMatrix> answer;
    
                    answer = RotationMethod(A, eps);
                    std::cout << "Eigenvalues:" << std::endl;
    
                    sizeM = answer.first.GetSA();
                    
                    for(size_t k = 0; k < sizeM; ++k){
                        std::cout << '[' << answer.first[k][k] << ']';
                        if(k + 1 != sizeM) {
                            std::cout << ", ";
                        }
                    }
                    std::cout << std::endl;
                    
                    std::cout << "Eigenvectors matrix:" << std::endl;
                    std::cout << answer.second;
    
                    std::cout << "Check? - ";
                    std::cin >> cont;
                    if(cont == 'y') {
                        TMatrix V1, V2;
                        for(size_t j1 = 0; j1 < sizeM; ++j1) {
                            for(size_t j2 = j1 + 1; j2 < sizeM; ++j2) {
                                answer.second.PrintPart(0, j1, sizeM - 1, j1);
                                answer.second.PrintPart(0, j2, sizeM - 1, j2);
                                std::cout << "(V" << j1 << ", V" << j2 << ") = " << V1.T()*V2;
                            }
                        }
                    }
                    std::cout << "End!" << std::endl;
                    break;
                }
                case '5':
                {
                    if(!matrices[0] || !matrices[1]) {
                        std::cout << "First you need to input:" << std::endl;
                        std::cout << "- Matrix A" << std::endl;
                        inTask = false;
                        break;
                    }
                    
                    double eps;
                    size_t sizeM;
                    std::cout << "Input epsilon: ";
                    std::cin >> eps;
                    std::pair<TMatrix, TMatrix> answer;
    
                    answer = QRAlgorithm(A, eps);
                    std::cout << "Eigenvalues:" << std::endl;
                    sizeM = answer.second.GetSA();
                    
                    for(size_t k = 0; k < sizeM; ++k){
                        std::cout << '[' << answer.second[k][k] << ']';
                        if(k + 1 != sizeM) {
                            std::cout << ", ";
                        }
                    }
                    std::cout << std::endl;
    
                    std::cout << "Check? - ";
                    std::cin >> cont;
                    if(cont == 'y') {
                        std::cout << "A:" << std::endl;
                        std::cout << A;
                        std::cout << "Q:" << std::endl;
                        std::cout << answer.first;
                        std::cout << "R:" << std::endl;
                        std::cout << answer.second;
                        std::cout << "Q*R:" << std::endl;
                        std::cout << answer.first * answer.second;
                        
                    }
                    std::cout << "End!" << std::endl;
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
