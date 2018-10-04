#include <cstdlib>
#include "Hexagon.h"
#include "Pentagon.h"
#include "Rhombus.h"
#include "TStack.h"
#include "TStackItem.h"

void ChoiseForStack() {
  TStack stack;
  char choice;
  for(;;) {
        std::cin >> choice;
        switch(choice) {
            case '1':
                std::cout << "Enter the diagonals:" << std::endl;
                stack.push(Rhombus(std::cin));
                break;
            case '2':
                break;
            case '3':
                std::cout << stack;
                break;
            case 'h':
                std::cout << "What to do:\n1. Add item in Stack\n2. Delete item in Stack.\n3. Show items in Stack\nType 'e' for exit from menu" << std::endl;
                break;
            case 'e':
                return;
            default:
                std::cout << "Error! For help type 'h'!" << std::endl;
                break;
        }
    }
    return;
}

int main(int argc, char** argv) {
    char choice;
    Figure *ptr;
    std::cout << "Choose the Function:\n1. Hexagon\n2. Pentagon\n3. Rhombus\n4. Use Stack for rhombus" << std::endl;
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
            case '4':
                std::cout << "What to do:\n1. Add item in Stack\n2. Delete item in Stack.\n3. Show items in Stack" << std::endl;
            case 'e':
                std::cout << "Exiting the program" << std::endl;
                return 0;
            case 'h':
                std::cout << "Choose the Function:\n1. Hexagon\n2. Pentagon\n3. Rhombus\n4. Use Stack for rhombus" << std::endl;
                ChoiseForStack();
                break;
            default:
                std::cout << "Error! For help type 'h'!" << std::endl;
                break;
        }
    }
    return 0;
}

