#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cctype>
#include <chrono>
#include <random>
#include <map>
#include "TPatriciaTrie.h"

void StrToLower(char* str) {
    int step = 0;
    while(str[step] != '\0' && str[step] != EOF && str[step] != '\n' && str[step] != ' ') {
        str[step] = tolower(str[step]);
        step++;
    }
    return;
}

double Benchmark(TPatriciaTrie<unsigned long long int>* tree, unsigned long long int numbWords) {
    
    char**                                  words = (char**) malloc(sizeof(char*)*numbWords);
    char                                    str[256];
    std::default_random_engine              generator;
    std::uniform_int_distribution<char>     character(97, 122);
    unsigned int start, finish, begin, end;
    double aTime;

    for(int i = 0; i < numbWords; ++i) {
        int numb = 127;
        words[i] = (char*) malloc((numb + 1)*sizeof(char));
        for(int j = 0; j < numb; ++j) {
            words[i][j] = character(generator);
        }
        words[i][numb] = '\0';
    }
    
    start = clock();
    
    begin = clock();
    for(unsigned long int i = 0; i < numbWords; ++i) {
        tree->Insert(words[i], i + 1);
    }
    end = clock();
    aTime = (double) (end - begin) / CLOCKS_PER_SEC;
    std::cout << "Avr. Time (Insert) - " << aTime << " sec" << std::endl;
    
    begin = clock();
    for(unsigned long int i = 0; i < numbWords; ++i) {
        tree->Lookup(words[i]);
    }
    end = clock();
    aTime = (double) (end - begin) / CLOCKS_PER_SEC;
    std::cout << "Avr. Time (Search) - " << aTime << " sec" << std::endl;



    std::ofstream file1("Benchmark_Current.bin", std::ofstream::binary);
    if(!file1) {
        std::cout << "ERROR: can't open file!" << std::endl;
        exit(0);
    }
    begin = clock();
    tree->SaveTrieCurrent(tree->GetHead(), &file1);
    end = clock();
    file1.close();
    aTime = (double) (end - begin) / CLOCKS_PER_SEC;
    std::cout << "Avr. Time (Save) - " << aTime << " sec" << std::endl;
    
    
    
    std::ifstream file4("Benchmark_Current.bin", std::ofstream::binary);
    if(!file4) {
        std::cout << "ERROR: can't open file!" << std::endl;
        exit(0);
    }
    if(!tree->Empty()) {
        tree->ClearTrie();
    }
    begin = clock();
    tree->LoadTrieCurrent(str, tree->GetHead(), &file4);
    end = clock();
    file4.close();
    aTime = (double) (end - begin) / CLOCKS_PER_SEC;
    std::cout << "Avr. Time (Load) - " << aTime << " sec" << std::endl;
    
    for(unsigned long int i = 0; i < numbWords; ++i) {
        tree->Lookup(words[i]);
    }
    
    begin = clock();
    for(unsigned long int i = 0; i < numbWords; i += 1) {
            tree->Delete(words[i]);
    }
    end = clock();
    aTime = (double) (end - begin) / CLOCKS_PER_SEC;
    std::cout << "Avr. Time (Delete) - " << aTime << " sec" << std::endl;
    
    
    
    finish = clock();
    std::cout << "All time - " <<  (double) (finish - start) / CLOCKS_PER_SEC << std::endl;
    
    std::map<char*, unsigned long long int> myMap;
    
    start = clock();
    
    begin = clock();
    for(unsigned long int i = 0; i < numbWords; ++i) {
        myMap.insert(std::pair<char*, unsigned long long int>(words[i],i + 1));
    }
    end = clock();
    aTime = (double) (end - begin) / CLOCKS_PER_SEC;
    std::cout << "Avr. Time (Insert) - " << aTime << " sec" << std::endl;
    
    begin = clock();
    for(unsigned long int i = 0; i < numbWords; ++i) {
            myMap.find(words[i]);
    }
    end = clock();
    aTime = (double) (end - begin) / CLOCKS_PER_SEC;
    std::cout << "Avr. Time (Search) - " << aTime << " sec" << std::endl;
    
    begin = clock();
    for(unsigned long int i = 0; i < numbWords; i += 1) {
        myMap.erase(words[i]);
    }
    end = clock();
    aTime = (double) (end - begin) / CLOCKS_PER_SEC;
    std::cout << "Avr. Time (Delete) - " << aTime << " sec" << std::endl;
    
    finish = clock();
    std::cout << "All time - " <<  (double) (finish - start) / CLOCKS_PER_SEC << std::endl;
    
    for(int i = 0; i < numbWords; ++i) {
        free(words[i]);
    }
    
    free(words);
    return 0;
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
    
    return;
}

void SaveOrLoad(TPatriciaTrie<unsigned long long int>* tree, char* str) {
    
    std::cin >> str;
    if(std::cin.eof()) {
        std::cout << "ERROR: EOF recieved!" << std::endl;
        exit(0);
    }
    
    if(!strcmp("Save", str)) {
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
        tree->SaveTrieCurrent(tree->GetHead(), &file);
        std::cout << "OK" << std::endl;
        file.close();
    }
    else if(!strcmp("Load", str)) {
        TPatriciaTrie<unsigned long long int>  tmp;
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
        
        tmp.LoadTrieCurrent(str, tmp.GetHead(), &file);
        tree->SwapHead(&tmp);
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

    TPatriciaTrie<unsigned long long int>   tree;
    //unsigned long long int                  numb = 0;
    char str[256];
    /*while(1) {
        std::cin >> numb;
        if(std::cin.eof()) {
            break;
        }
        Benchmark(&tree, numb);
    }*/ 
    
    while(true) {
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
    }
    return 0;
}
