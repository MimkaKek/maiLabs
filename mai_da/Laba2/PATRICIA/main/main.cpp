#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cctype>
#include <chrono>
#include <random>
#include "TPatriciaTrie.h"

const unsigned long long int MAX_WORDS = 10000;

void StrToLower(char* str) {
    int step = 0;
    while(str[step] != '\0' && str[step] != EOF && str[step] != '\n' && str[step] != ' ') {
        str[step] = tolower(str[step]);
        step++;
    }
    return;
}

void Benchmark(TPatriciaTrie<unsigned long long int>* tree) {
    
    char*                                   words[MAX_WORDS];
    char                                    tags[MAX_WORDS];
    std::default_random_engine              generator;
    std::uniform_int_distribution<char>     character(97, 122);
    double bs, cs, bl, cl;

    for(int i = 0; i < MAX_WORDS; ++i) {
        int numb = 200;
        words[i] = (char*) malloc((numb + 1)*sizeof(char));
        for(int j = 0; j < numb; ++j) {
            words[i][j] = character(generator);
        }
        words[i][numb] = '\0';
    }
    
    unsigned int start_time =  clock();
    
    for(unsigned long int i = 0; i < MAX_WORDS; ++i) {
        if(tree->Insert(words[i], i + 1)) {
            tags[i] = 1;
        }
        else {
            tags[i] = 0;
        }
    }
    
    for(unsigned long int i = 0; i < MAX_WORDS; ++i) {
        if(tags[i]) {
            if(!tree->Lookup(words[i])) {
                std::cout << "Can't find word - " << words[i] << std::endl << "Number - " << i + 1 << std::endl;
                exit(0);
            }
        }
    }
    
    unsigned int save_time = clock();
    {
        std::ofstream file("Benchmark_Before.bin", std::ofstream::binary);
        if(!file) {
            std::cout << "ERROR: can't open file!" << std::endl;
            exit(0);
        }
        tree->SaveTrie_Before(tree->GetHead(), &file);
        file.close();
    }
    unsigned int end_time = clock();
    unsigned int search_time = (end_time - save_time);
    bs = (double) search_time / CLOCKS_PER_SEC;
    std::cout << "Save_Before - " << bs << std::endl;
    
    save_time = clock();
    {
        std::ofstream file("Benchmark_Current.bin", std::ofstream::binary);
        if(!file) {
            std::cout << "ERROR: can't open file!" << std::endl;
            exit(0);
        }
        tree->SaveTrie_Current(tree->GetHead(), &file);
        file.close();
    }
    end_time = clock();
    search_time = (end_time - save_time);
    cs = (double) search_time / CLOCKS_PER_SEC;
    std::cout << "Save_Current - " << cs << std::endl;
    
    save_time = clock();
    {
        std::ifstream file("Benchmark_Before.bin", std::ofstream::binary);
        if(!file) {
            std::cout << "ERROR: can't open file!" << std::endl;
            exit(0);
        }
        if(!tree->Empty()) {
            tree->ClearTrie();
        }
        char str[256];
        tree->LoadTrie_Before(str, tree->GetHead(), &file);
        file.close();
    }
    end_time = clock();
    search_time = (end_time - save_time);
    bl = (double) search_time / CLOCKS_PER_SEC;
    std::cout << "Load_Before - " << bl << std::endl;
    
    for(unsigned long int i = 0; i < MAX_WORDS; ++i) {
        if(tags[i]) {
            if(!tree->Lookup(words[i])) {
                std::cout << "Can't find word - " << words[i] << std::endl << "Number - " << i + 1 << std::endl;
                exit(0);
            }
        }
    }
    
    save_time = clock();
    {
        std::ifstream file("Benchmark_Current.bin", std::ofstream::binary);
        if(!file) {
            std::cout << "ERROR: can't open file!" << std::endl;
            exit(0);
        }
        if(!tree->Empty()) {
            tree->ClearTrie();
        }
        char str[256];
        tree->LoadTrie_Current(str, &file);
        file.close();
    }
    end_time = clock();
    search_time = (end_time - save_time);
    cl = (double) search_time / CLOCKS_PER_SEC;
    std::cout << "Load_Current - " << cl << std::endl;
    
    for(unsigned long int i = 0; i < MAX_WORDS; ++i) {
        if(tags[i]) {
            if(!tree->Lookup(words[i])) {
                std::cout << "Can't find word - " << words[i] << std::endl << "Number - " << i + 1 << std::endl;
                exit(0);
            }
        }
    }
    
    std::cout << "Total before - " << bl + bs << std::endl;
    std::cout << "Total current - " << cl + cs << std::endl;
    
    for(unsigned long int i = 0; i < MAX_WORDS; i += 1) {
        if(tags[i]) {
            if(tree->Delete(words[i])) {
                tags[i] = 0;
            }
            else {
                std::cout << "Error detected - Delete: " << words[i] << std::endl;
            }
        }
    }
    
    end_time = clock();
    search_time = (end_time - start_time);
    std::cout << "All time - " <<  (double) search_time / CLOCKS_PER_SEC << std::endl;
    
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
        tree->SaveTrie_Current(tree->GetHead(), &file);
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
        tree->LoadTrie_Current(str, &file);
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
