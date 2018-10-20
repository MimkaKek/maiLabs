#include <cstdlib>
#include "Rhombus.h"
#include "TArray.h"

int main(int argc, char** argv) {
    char choice;
    TArray array;
    Rhombus* tmp = NULL;
    std::cout << "Choose the Function:\n1. Add\n2. Delete\n3. Print\ne. Close" << std::endl;
    for(;;) {
        std::cin >> choice;
        switch(choice) {
            case '1':
                std::cout << "Enter the diagonals of rhombus:";
                tmp = new Rhombus(std::cin);
                array.Add(*tmp);
                delete tmp;
                break;
            case '2':
                std::cout << "Enter the diagonals of rhombus:";
                tmp = new Rhombus(std::cin);
                array.Remove(*tmp);
                delete tmp;
                break;
            case '3':
				        std::cout << array << std::endl;
				        break;
            case 'e':
                std::cout << "Exiting the program" << std::endl;
                return 0;
            case 'h':
                std::cout << "Choose the Function:\n1. Hexagon\n2. Pentagon\n3. Rhombus\n4. Use Stack for rhombus" << std::endl;
                break;
            default:
                std::cout << "Error! For help type 'h'!" << std::endl;
                break;
        }
    }
    return 0;
}

