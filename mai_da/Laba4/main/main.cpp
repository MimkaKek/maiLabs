#include <iostream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <random>
#include "TAhoCorasick.h"
#include "Settings.h"

const int NUMB_OF_ITERATIONS = 10;

void Benchmark() {
    
    for(int nPat = 10; nPat <= 1000; nPat *= 10) {
        for(int lPat = 25; lPat <= 100; lPat *= 2) {
            for(int nIn = 1000; nIn <= 100000; nIn *= 10) {
                unsigned long long int**                                    patterns = (unsigned long long int**) malloc(sizeof(unsigned long long int*)*nPat);
                int*                                                        lengPat = (int*) malloc(sizeof(int)*nPat);
                std::default_random_engine                                  generator;
                std::uniform_int_distribution<unsigned long long int>       partOfPattern(0, 1000);
                std::uniform_int_distribution<int>                          nWay(1, 10);
                unsigned int begin, end;
                double aTime = 0;
            
                for(int i = 0; i < nPat; ++i) {
                    lengPat[i] = lPat;
                    patterns[i] = (unsigned long long int*) malloc(lPat*sizeof(unsigned long long int));
                    for(int j = 0; j < lPat; ++j) {
                        patterns[i][j] = partOfPattern(generator);
                    }
                }
                
                unsigned long long int* input = (unsigned long long int*) malloc(sizeof(unsigned long long int)*nIn);
                int posPattern = 0;
                int posInput = 0;
                int n = 0;
                while(posInput < nIn) {
                    int act = nWay(generator);
                    if(act < 7) {
                        input[posInput] = partOfPattern(generator);
                        ++posInput;
                    }
                    else {
                        posPattern = 0;
                        n = partOfPattern(generator) % nPat;
                        while(posInput < nIn && posPattern < lengPat[n]) {
                            input[posInput] = patterns[n][posPattern];
                            ++posInput;
                            ++posPattern;
                        }
                    }
                }
                
                for(int iter = 0; iter < NUMB_OF_ITERATIONS; ++iter) {
                    TAhoCorasick ak;
                    
                    ak.ToStart();
                    
                    begin = clock();
                    for(int i = 0; i < nPat; ++i) {
                        for(int j = 0; j < lengPat[i]; ++j) {
                            ak.AddToPattern(patterns[i][j]);
                        }
                        ak.FinPattern();
                    }
                    
                    ak.Build();
                    
                    for(int i = 0; i < nIn; ++i) {
                        ak.Search(input[i], 1);
                    }
                    end = clock();
                    aTime += (double) (end - begin) / CLOCKS_PER_SEC;
                    
                }
                
                std::cout << "Number of Patterns: " << nPat << std::endl;
                std::cout << "Length of Patterns: " << lPat << std::endl;
                std::cout << "Length of Text: " << nIn << std::endl;
                std::cout << "Time: $" << aTime / NUMB_OF_ITERATIONS << "$" << std::endl;
                
                for(int i = 0; i < nPat; ++i) {
                    free(patterns[i]);
                }
                free(input);
                free(lengPat);
                free(patterns);
            }
        }
    }
}

int main() {
    
    if(BENCHMARK) {
        Benchmark();
    }
    else {

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
                    std::cout << "Get part of pattern = " << number << std::endl;
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
                ak.Search(number, strNumb);
            }
            if(!(translation.eof())) {
                std::cout << "ERROR: bad input!" << std::endl;
                return 0;
            }
        }
    }
    return 0;
}
