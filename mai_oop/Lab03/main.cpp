#include <cstdlib>
#include "Rhombus.h"
#include "Pentagon.h"
#include "Hexagon.h"
#include "TArray.h"

std::shared_ptr<Figure> Choise() {
    std::shared_ptr<Figure> ptr;
    char choice;
    std::cout << "==================\nChoose the Figure:\n1. Hexagon\n2. Pentagon\n3. Rhombus\ne. Exit" << std::endl;
    for(;;) {
        std::cout << "> ";
        std::cin >> choice;
        switch(choice) {
            case '1':
                std::cout << "Enter the side of Hexagon: ";
                ptr = std::make_shared<Hexagon> (Hexagon(std::cin));
                return ptr;
            case '2':
                std::cout << "Enter the side of Pentagon: ";
                ptr = std::make_shared<Pentagon> (Pentagon(std::cin));
                return ptr;
            case '3':
                std::cout << "Enter the diagonals of Rhombus: ";
                ptr = std::make_shared<Rhombus> (Rhombus(std::cin));
                return ptr;
            case 'e':
                return nullptr;
        }
    }
}

int main(int argc, char** argv) {
    char choice;
    TArray<Figure> array;
    std::shared_ptr<Figure> tmp = NULL;
    for(;;) {
        std::cout << "==================\nChoose the Function:\n1. Add\n2. Delete\n3. Print\ne. Close" << std::endl;
        std::cout << "> ";
        std::cin >> choice;
        switch(choice) {
            case '1':
                array.Add(Choise());
                break;
            case '2':
                int i;
                std::cout << "Enter the number of element: ";
                std::cin >> i;
                tmp = array.Remove(i);
                tmp = nullptr;
                break;
            case '3':
                std::cout << "==================" << std::endl;
                std::cout << array;
                break;
            case 'e':
                std::cout << "Exiting the program" << std::endl;
                return 0;
            case 'h':
                std::cout << "=\nChoose the Function:\n1. Hexagon\n2. Pentagon\n3. Rhombus\n4. Use Stack for rhombus" << std::endl;
                break;
            default:
                std::cout << "Error! For help type 'h'!" << std::endl;
                break;
        }
    }
    return 0;
}

