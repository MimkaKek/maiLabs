#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cctype>
#include <chrono>
#include <random>
#include "TPatriciaTrie.h"

const unsigned long long int MAX_WORDS = 1000;

void StrToLower(char* str) {
    int step = 0;
    while(str[step] != '\0' && str[step] != EOF && str[step] != '\n' && str[step] != ' ') {
        str[step] = tolower(str[step]);
        step++;
    }
    return;
}

void Benchmark(TPatriciaTrie<unsigned long long int>* tree) {
    
    unsigned long int                       state = 1;
    int                                     error = 0;
    char*                                   words[MAX_WORDS];
    char                                    tags[MAX_WORDS];
    std::chrono::high_resolution_clock::time_point beginning = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::duration d = std::chrono::high_resolution_clock::now() - beginning;
    unsigned seed2 = d.count();
    std::default_random_engine              generator(seed2);
    std::uniform_int_distribution<int>      lengthOfWord(50, 100);
    std::uniform_int_distribution<char>     character(97, 122);

    for(int i = 0; i < MAX_WORDS; ++i) {
        int numb = lengthOfWord(generator);
        words[i] = (char*) malloc((numb + 1)*sizeof(char));
        for(int j = 0; j < numb; ++j) {
            words[i][j] = character(generator);
        }
        words[i][numb] = '\0';
    }
    
    unsigned int start_time =  clock();
    
    for(unsigned long int i = 0; i < MAX_WORDS; ++i) {
        if(tree->Insert(words[i], i)) {
            tags[i] = 1;
            if(!tree->Lookup(words[i])) {
                std::cout << "Error detected - Search after insert: " << words[i] << std::endl;
            }
            std::cout << "=========================================" << std::endl;
            std::cout << "|\tWORD: " << words[i] << "\t\t\t\t|" << std::endl;
            std::cout << "|\tNUMB: " << i << "\t\t\t\t|" << std::endl;
            std::cout << "=========================================" << std::endl;
            std::cout << "|\tSTATE #" << state << "\t\t\t|" << std::endl;
            std::cout << "|\tINSERT" << "\t\t\t\t|" << std::endl;
            std::cout << "=========================================" << std::endl;
            ++state;
            tree->Print(tree->GetHead(), 0);
                if(i == MAX_WORDS - 1) {
                    std::cout << "=========================================" << std::endl;
                }
            for(int j = 0; j <= i; ++j) {
                if(tags[j]) {
                    if(!tree->Lookup(words[j])) {
                        if(!error) {
                            std::cout << "=========================================" << std::endl;
                        }
                        std::cout << "Error detected - Search after insert: " << words[j] << " = " << j << std::endl;
                        error = 1;
                    }
                }
            }
            if(error) {
                std::cout << "=========================================" << std::endl;
                exit(0);
            }
        }
        else {
            tags[i] = 0;
            if(!(tree->Lookup(words[i]))) {
                std::cout << "=========================================" << std::endl;
                std::cout << "Error detected - Search after insert: " << words[i] << " = " << i << std::endl;
                error = 1;
            }
        }
    }
    
    for(unsigned long int i = 0; i < MAX_WORDS; i += 1) {
            if(tree->Delete(words[i])) {
                if(i) {
                    std::cout << "=========================================" << std::endl;   
                }
                std::cout << "|\tWORD: " << words[i] << "\t\t\t\t|" << std::endl;
                std::cout << "|\tNUMB: " << i << "\t\t\t\t|" << std::endl;
                std::cout << "=========================================" << std::endl;
                std::cout << "|\tSTATE #" << state << "\t\t\t|" << std::endl;
                std::cout << "|\tDELETE" << "\t\t\t\t|" << std::endl;
                std::cout << "=========================================" << std::endl;
                tree->Print(tree->GetHead(), 0);
                if(i == MAX_WORDS - 1) {
                    std::cout << "=========================================" << std::endl;
                }
                ++state;
                tags[i] = 0;
                for(int j = 0; j < MAX_WORDS; ++j) {
                    if(tags[j]) {
                        if(!tree->Lookup(words[j])) {
                            if(!error) {
                                std::cout << "=========================================" << std::endl;
                            }
                            std::cout << "Error detected - Search after delete: " << words[j] << " = " << j << std::endl;
                            error = 1;
                        }
                    }
                }
                if(error) {
                    std::cout << "=========================================" << std::endl;
                    exit(0);
                }
            }
            else {
                std::cout << "Error detected - Delete: " << words[i] << std::endl;
            }
    }
    
    unsigned int end_time = clock();
    unsigned int search_time = (end_time - start_time);
    std::cout <<  (double) search_time / CLOCKS_PER_SEC << std::endl;
    for(int i = 0; i < MAX_WORDS; ++i) {
        free(words[i]);
    }
    std::cout << "SUCCESS" << std::endl;
    return;
}

void AddInTree(TPatriciaTrie<unsigned long long int>* tree, char* str) {
    
    unsigned long long int numb = 0;
    int step = 0;
    
    
    std::cin >> str;
    if(std::cin.eof()) {
        std::cout << "ERROR: EOF recieved!" << std::endl;
        exit(0);
    }
    
    while(str[step] != '\0') {
        if(str[step] >= '0' && str[step] <= '9') {
            std::cout << "ERROR: Number recieved, but we need a character!" << std::endl;
            exit(0);
        }
        ++step;
    }
    
    StrToLower(str);
    
    if(!(std::cin >> numb)) {
        std::cout << "ERROR: Incorrect input!" << std::endl;
        exit(0);
    }
    
    if(tree->Insert(str, numb)) {
        puts("OK");
    }
    else {
        std::cout << "Exist" << std::endl;
    }
    
    //tree->Print(tree->GetHead(), 0);
    return;
}

void RemoveFromTree(TPatriciaTrie<unsigned long long int>* tree, char* str) {
    
    int step = 0;
    
    
    std::cin >> str;
    if(std::cin.eof()) {
        std::cout << "ERROR: EOF recieved!" << std::endl;
        exit(0);
    }
    
    while(str[step] != '\0') {
        if(str[step] >= '0' && str[step] <= '9') {
            std::cout << "ERROR: Number recieved, but we need a character!" << std::endl;
            exit(0);
        }
        ++step;
    }
    
    StrToLower(str);
    if(tree->Delete(str)) {
        puts("OK");
    }
    else {
        std::cout << "NoSuchWord" << std::endl;
    }
    //tree->Print(tree->GetHead(), 0);
    return;
}

void SaveOrLoad(TPatriciaTrie<unsigned long long int>* tree, char* str) {
    
    
    std::cin >> str;
    if(std::cin.eof()) {
        std::cout << "ERROR: EOF recieved!" << std::endl;
        exit(0);
    }
    
    if(!strcmp("Exit", str)) {
        exit(EXIT_SUCCESS);
    }
    else if(!strcmp("Save", str)) {
        std::cin >> str;
        if(std::cin.eof()) {
            std::cout << "ERROR: EOF recieved!" << std::endl;
            exit(0);
        }
        std::ofstream file(str, std::ofstream::binary);
        if(!file) {
            std::cout << "ERROR: can't open file!" << std::endl;
            exit(0);
        }
        tree->SaveTrie(tree->GetHead(), &file);
        std::cout << "OK" << std::endl;
        file.close();
    }
    else if(!strcmp("Load", str)) {
        std::cin >> str;
        if(std::cin.eof()) {
            std::cout << "ERROR: EOF recieved!" << std::endl;
            exit(0);
        }
        std::ifstream file(str, std::ofstream::binary);
        if(!file) {
            std::cout << "ERROR: can't open file!" << std::endl;
            exit(0);
        }
        if(!tree->Empty()) {
            tree->ClearTrie();
        }
        tree->LoadTrie(str, tree->GetHead(), &file);
        std::cout << "OK" << std::endl;
        file.close();
    }
    else {
        std::cout << "ERROR: Wrong command after '!'" << std::endl;
        exit(0);
    }
    return;
}

void SearchInTree(TPatriciaTrie<unsigned long long int>* tree, char* str) {
    
    unsigned long long int* answer;
    StrToLower(str);
    answer = tree->Lookup(str);
    
    if(!answer) {
        puts("NoSuchWord");
    }
    else {
        std::cout << "OK: " << *answer << std::endl;
    }
    return;
}

int main(int argc, char** argv) {

    TPatriciaTrie<unsigned long long int> tree;
    //char str[256];
    Benchmark(&tree);
    /*while(true) {
        std::cin >> str;
        if(std::cin.eof()) {
            break;
        }
        switch(str[0]) {
            case '+':
                AddInTree(&tree, str);
                break;
            case '-':
                RemoveFromTree(&tree, str);
                break;
            case '!':
                SaveOrLoad(&tree, str);
                break;
            default:
                SearchInTree(&tree, str);
        }
    }*/
    return 0;
}
