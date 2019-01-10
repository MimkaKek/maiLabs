#include <cstdlib>
#include <future>
#include <functional>
#include <random>
#include <thread>
#include "Rhombus.h"
#include "Pentagon.h"
#include "Hexagon.h"
#include "TArray.h"

void ParallelSort(TArray<Figure>* array) {
    if (array->Size() > 1) {
        std::shared_ptr<Figure> middle = array->Pop(0);
        TArray<Figure> left, right;

        while (!array->Empty()) {
            std::shared_ptr<Figure> item = array->Pop(0);
            if (*item < *middle) {
                left.Push(item);
            } else {
                right.Push(item);
            }
        }

        std::future<void> left_res = std::async(std::launch::async, ParallelSort, &left);
        std::future<void> right_res = std::async(std::launch::async, ParallelSort, &right);

        left_res.get();

        while (!left.Empty()) { 
            array->Push(left.Pop(0));
        }
        
        array->Push(middle);
        
        right_res.get();
        
        while (!right.Empty()) { 
            array->Push(right.Pop(0));
        }
    }
    return;
}

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
    std::shared_ptr<Figure>  tmp = nullptr;
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, 1000);
    std::uniform_int_distribution<int> param(1, 3);
    int side1;
    int side2;
    int fig;
    
    for(;;) {
        std::cout << "==================\nChoose the Function:\n1. Add\n2. Delete\n3. Sort array\n4. Print\ne. Close" << std::endl;
        std::cout << "> ";
        std::cin >> choice;
        switch(choice) {
            case '1':
                //*
                for (int i = 0; i < 20; i++) {
                    side1 = distribution(generator);
                    side2 = distribution(generator);
                    fig = param(generator);
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
                }//*/
                //(array.Push(Choise())) ? puts("Figure succesfully added in array!") : puts("Failed!");
                break;
            case '2':
                int i;
                std::cout << "Enter the number of element: ";
                std::cin >> i;
                tmp = array.Pop(i - 1);
                tmp = nullptr;
                break;
            case '3':
                std::cout << "Parallel sort? (1 or 0): ";
                std::cin >> i;
                if(i == 1) {
                    ParallelSort(&array);
                }
                else if (i == 0) {
                    array.Sort();
                }
                else {
                    std::cout << "Error: bad format!" << std::endl;
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
                std::cout << "==================\nChoose the Function:\n1. Add\n2. Delete\n3. Sort array\n4. Print\ne. Close" << std::endl;
                break;
            default:
                std::cout << "Error! For help type 'h'!" << std::endl;
                break;
        }
    }
    return 0;
}

