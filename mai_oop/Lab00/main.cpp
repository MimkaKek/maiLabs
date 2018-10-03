#include <cstdlib>
#include "Hexagon.h"
#include "Pentagon.h"
#include "Rhombus.h"

int main(int argc, char** argv) {        
    std::cout << "Choose the Figure:\n1. Hexagon\n2. Pentagon\n3. Rhombus" << std::endl;
    char choice;
    Figure *ptr;
    bool key = true;
    for(;;) {
        std::cin >> choice;
        switch(choice) {
            case '1':
                std::cout << "Enter the side: ";
                ptr = new Hexagon(std::cin);
                ptr->Print();
                ptr->Square();
                delete ptr;
                break;
            case '2':
                std::cout << "Enter the side: ";
                ptr = new Pentagon(std::cin);
                ptr->Print();
                ptr->Square();
                delete ptr;
                break;
            case '3':
                std::cout << "Enter 2 diagonals: ";
                ptr = new Rhombus(std::cin);
                ptr->Print();
                ptr->Square();
                delete ptr;
            case 'e':
                std::cout << "Exiting the program" << std::endl;
                return 0;
            case 'h':
                std::cout << "Choose the Figure:\n1. Hexagon\n2. Pentagon\n3. Rhombus" << std::endl;
                break;
//           case '\n':
//               key = true;
            default:
//                if(key) {
//                    key = false;
                    std::cout << "Error! For help type 'h'!" << std::endl;
//                }
                break;
        }
    }
    return 0;
}

