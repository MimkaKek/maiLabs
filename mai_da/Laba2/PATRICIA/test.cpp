#include <stdio.h>
#include <string>
#include "patricia.h"

void StrToLower(char* str) {
    int step = 0;
    while(str[step] != '\0' && str[step] != EOF) {
        str[step] = tolower(str[step]);
        step++;
    }
    return;
}

int main(int argc, char* argv[]) {

    nPatriciaTrie<unsigned long long int>* tree = new nPatriciaTrie<unsigned long long int>();
    unsigned long long int key = 0;
    //char pathToFile[256];
    char str[256];
    //FILE* file;
    
    while(scanf("%s", str) != EOF) {
        switch(str[0]) {
            case '\n':
                break;
            case '+':
                if(scanf("%s%llu", str, &key) == 2) {
                    StrToLower(str);
                    tree->Insert(str, key);
                    puts("OK");
                }
                else {
                    puts("ERROR: bad format!");
                    return 0;
                }
                break;
            case '-':
                if(scanf("%s", str)) {
                    StrToLower(str);
                    tree->Delete(str);
                }
                else {
                    puts("ERROR: bad format!");
                    return 0;
                }
                break;
            /*case '!':
                if(scanf("%s", str) == 1) {
                    if(strcmp(str,"Save") == 0) {
                        scanf("%s", pathToFile);
                        file = fopen(pathToFile, "w");
                        if(file == NULL) {
                            puts("ERROR: Can't open the file!");
                            return 0;
                        }
                        FSaveTree(file, root, str, 0);
                        puts("OK");
                        fclose(file);
                    }
                    else if(strcmp(str,"Load") == 0) {
                        scanf("%s", pathToFile);
                        file = fopen(pathToFile, "r");
                        if(file == NULL) {
                            puts("ERROR: Can't open the file!");
                            return 0;
                        }
                        tmp = FLoadTree(file, tmp, str);
                        if(answer == 2) {
                            answer = 0;
                            tmp = ClearTree(tmp);
                            continue;
                        }
                        root = ClearTree(root);
                        root = tmp;
                        fclose(file);
                        puts("OK");
                    }
                    else if(strcmp(str, "Exit") == 0) {
                        root = ClearTree(root);
                        return 0;
                    }
                    else if(strcmp(str, "Print") == 0) {
                        PrintTree(root, 0);
                        break;
                    }
                    else {
                        puts("ERROR: wrong command!");
                        return 0;
                    }
                }
                else {
                    puts("ERROR: bad input format!");
                    return 0;
                }
                break;*/
            default:
                StrToLower(str);
                unsigned long long int* tmp = tree->Lookup(str);
                if(tmp == NULL) {
                    puts("NoSuchWord");
                    continue;
                }
                printf("OK: %llu\n", *tmp);
                break;
        }
    }
    delete tree;

    return 0;
}
