#include <cstdlib>
#include "Rhombus.h"
#include "Pentagon.h"
#include "Hexagon.h"
#include "TArray.h"

std::shared_ptr<Figure> Choise() {
    std::shared_ptr<Figure> ptr;
    char choice;
    std::cout << "Choose the Figure:\n1. Hexagon\n2. Pentagon\n3. Rhombus\ne. Exit" << std::endl;
    for(;;) {
        std::cin >> choice;
        switch(choice) {
            case '1':
                std::cout << "Enter the side of Hexagon: ";
                return ptr = std::make_shared<Hexagon> (Hexagon(std::cin));
                break;
            case '2':
                std::cout << "Enter the side of Pentagon: ";
                return ptr = std::make_shared<Pentagon> (Pentagon(std::cin));
                break;
            case '3':
                std::cout << "Enter the diagonals of Rhombus: ";
                return ptr = std::make_shared<Rhombus> (Rhombus(std::cin));
                break;
            case 'e':
                return nullptr;
        }
    }
}

int main(int argc, char** argv) {
    char choice;
    TArray array;
    //Figure* tmp = NULL;
    std::cout << "Choose the Function:\n1. Add\n2. Delete\n3. Print\ne. Close" << std::endl;
    for(;;) {
        std::cin >> choice;
        switch(choice) {
            case '1':
                array.Add(Choise());
                break;
            case '2':
                array.Remove(Choise());
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

