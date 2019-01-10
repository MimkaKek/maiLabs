#include <cstdlib>
#include <future>
#include <functional>
#include <random>
#include <thread>
#include "Rhombus.h"
#include "Pentagon.h"
#include "Hexagon.h"
#include "TArray.h"
#include "TQueue.h"

typedef std::function<void(void)> command;

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
                ptr = std::shared_ptr<Hexagon> (new Hexagon(std::cin));
                return ptr;
            case '2':
                std::cout << "Enter the side of Pentagon: ";
                ptr = std::shared_ptr<Pentagon> (new Pentagon(std::cin));
                return ptr;
            case '3':
                std::cout << "Enter the diagonals of Rhombus: ";
                ptr = std::shared_ptr<Rhombus> (new Rhombus(std::cin));
                return ptr;
            case 'e':
                return nullptr;
        }
    }
}

int main(int argc, char** argv) {
    
    char                     choice;
    int                      number = 0;
    TArray<Figure>           array;
    TQueue<command>          queue;
    std::shared_ptr<Figure>  tmp = nullptr;
    
    command cmd_insert = [&]() {
        std::cout << "Command: Create triangles" << std::endl;
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(1, 1000);
        std::uniform_int_distribution<int> param(1, 3);

        for (int i = 0; i < 10; i++) {
            int side1 = distribution(generator);
            int side2 = distribution(generator);
            int fig = param(generator);
            switch(fig) {
                case 1:
                    array.Push(std::shared_ptr<Hexagon> (new Hexagon(side1)));
                    break;
                case 2:
                    array.Push(std::shared_ptr<Pentagon> (new Pentagon(side2)));
                    break;
                case 3:
                    array.Push(std::shared_ptr<Rhombus> (new Rhombus(side1, side2)));
                    break;
            }
        }
    };

    command cmd_print = [&]() {
        std::cout << "Command: Print array" << std::endl;
        std::cout << array;
    };


    command cmd_reverse = [&]() {
        std::cout << "Command: Reverse array" << std::endl;
        array.Reverse();
    };

    queue.push(&cmd_print);
    queue.push(&cmd_reverse);
    queue.push(&cmd_print);
    queue.push(&cmd_insert);
    
    for(;;) {
        std::cout << "==================\nChoose the Function:\n1. Add\n2. Delete\n3. Next command\n4. Print\ne. Close" << std::endl;
        std::cout << "> ";
        std::cin >> choice;
        switch(choice) {
            case '1':
                (array.Push(Choise())) ? puts("Figure succesfully added in array!") : puts("Failed!");
                break;
            case '2':
                int i;
                std::cout << "Enter the number of element: ";
                std::cin >> i;
                tmp = array.Pop(i - 1);
                tmp = nullptr;
                break;
            case '3':
                if(!queue.empty()) {
                    command* cmd = queue.pop();
                    std::future<void> ft = std::async(*cmd);      
                    ft.get();
                    //std::thread(*cmd).detach();
                }
                break;
            case '4':
                std::cout << "==================" << std::endl;
                number = 0;
                for(auto i : array) {
                    ++number;
                    std::cout << number << ". " << *i << std::endl;
                }
                break;
            case 'e':
                std::cout << "Exiting the program" << std::endl;
                return 0;
            case 'h':
                std::cout << "==================\nChoose the Function:\n1. Add\n2. Delete\n3. Next command\n4. Print\ne. Close" << std::endl;
                break;
            default:
                std::cout << "Error! For help type 'h'!" << std::endl;
                break;
        }
    }
    return 0;
}

