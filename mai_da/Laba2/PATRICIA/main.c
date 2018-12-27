#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int answer = 0;

typedef struct Node {
    long long int  bit;
    unsigned long long int  key;
    char*                   Word;
    struct Node*            left;
    struct Node*            right;
} TNode;

void StrToLower(char* str) {
    int step = 0;
    while(str[step] != '\0' && str[step] != EOF) {
        str[step] = tolower(str[step]);
        step++;
    }
    return;
}

TNode* AddNode(unsigned long long int key, char* str) {
    TNode* tmp = (TNode*) malloc(sizeof(TNode));
    if(tmp == NULL) {
        puts("ERROR: can't allocate memory for node!");
        exit(0);
    }
    tmp->partOfWord = malloc(sizeof(char)*(strlen(str) + 1));
    tmp->bit = -1;
    tmp->key = key;
    strcpy(tmp->partOfWord, str);
    tmp->left = NULL;
    tmp->right = NULL;
    return tmp;
}

TNode* AddInTree(TNode* node, char* word, unsigned long long int key, long long int bit) {
    if(bit <= node->bit) {
        
    }
}

int main() {
    unsigned long long int key = 0;
    char pathToFile[256];
    char str[256];
    FILE* file;
    TNode* root = NULL;
    TNode* tmp = NULL;
    while(scanf("%s", str) != EOF) {
        switch(str[0]) {
            case '\n':
                break;
            case '+':
                if(scanf("%s%llu", str, &key) == 2) {
                    StrToLower(str);
                    root = AddInTree(root, str, key);
                    if(answer == 1) {
                        puts("OK");
                        answer = 0;
                    }
                }
                else {
                    puts("ERROR: bad format!");
                    return 0;
                }
                break;
            case '-':
                if(scanf("%s", str)) {
                    StrToLower(str);
                    root = RemoveFromTree(root, str, 0);
                    if(answer == 0) {
                        puts("NoSuchWord");
                    }
                    answer = 0;
                }
                else {
                    puts("ERROR: bad format!");
                    return 0;
                }
                break;
            case '!':
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
                break;
            default:
                StrToLower(str);
                FindInTree(root, str);
                if(answer == 0) {
                    puts("NoSuchWord");
                }
                answer = 0;
                break;
        }
    }
}
