#include <iostream>
#include <sstream>
#include "TAhoCorasick.h"
#include "Settings.h"

int main() {
    unsigned long long int  number = 0;
    unsigned int            strNumb = 0;
    unsigned int            wordNumb = 0;
    std::string             word;
    TAhoCorasick            ak;
    
    ak.ToStart();
    while(true) {
        std::getline(std::cin, word);
        if(std::cin.eof()) {
            std::cout << "ERROR: bad input!" << std::endl;
            return 0;
        }
        if(word == "") {
            break;
        }
        std::stringstream translation(word);
        while (translation >> number) {
            if(DEBUG_GET_NUMB_PATTERN) {
                std::cout << "Get pattern number = " << number << std::endl;
            }
            ak.AddToPattern(number);
        }
        if(!(translation.eof())) {
            std::cout << "ERROR: bad input!" << std::endl;
            return 0;
        }
        ak.FinPattern();
    }

    ak.Build();
    
    while(true) {
        std::getline(std::cin, word);
        if(std::cin.eof()) {
            break;
        }
        ++strNumb;
        if(word == "") {
            ak.EmptyStr();
            continue;
        }
        std::stringstream translation(word);
        wordNumb = 0;
        while (translation >> number) {
            if(DEBUG_GET_NUMB_STR) {
                std::cout << "Get from string number = " << number << std::endl;
            }
            ++wordNumb;
            ak.Search(number, strNumb, wordNumb);
        }
        if(!(translation.eof())) {
            std::cout << "ERROR: bad input!" << std::endl;
            return 0;
        }
    }

    return 0;
}
