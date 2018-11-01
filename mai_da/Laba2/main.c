#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int answer = 0;

typedef struct Node {
    unsigned long long int key;
    char* partOfWord;
    struct Node* next;
    struct Node* neighbor;
} TNode;

TNode* AddNode(unsigned long long int key, char* str) {
    TNode* tmp = (TNode*) malloc(sizeof(TNode));
    if(tmp == NULL) {
        puts("Error!");
        exit(0);
    }
    tmp->key = key;
    tmp->partOfWord = malloc(sizeof(char)*(strlen(str) + 1));
    strcpy(tmp->partOfWord, str);
    tmp->next = NULL;
    tmp->neighbor = NULL;
    puts("OK");
    return tmp;
}

TNode* AddAnotherNode(unsigned long long int key, char* str, TNode* next) {
    TNode* tmp = (TNode*) malloc(sizeof(TNode));
    if(tmp == NULL) {
        puts("Error!");
        exit(0);
    }
    tmp->key = key;
    tmp->partOfWord = malloc(sizeof(char)*(strlen(str) + 2));
    strcpy(tmp->partOfWord, str);
    tmp->partOfWord[strlen(str) + 1] = '$';
    tmp->next = next;
    tmp->neighbor = NULL;
    return tmp;
}

void ClearTree(TNode* node) {
    if(node == NULL) {
        return;
    }
    ClearTree(node->next);
    ClearTree(node->neighbor);
    free(node->partOfWord);
    free(node);
    return;
}

TNode* FAddInTree(TNode* node, int deep, long long unsigned key, char* str) {
    if(node == NULL) {
        if(str[strlen(str) + 1] == '$') {
            node = AddAnotherNode(key, str, NULL);
        }
        else {
            node = AddNode(key, str);
        }
        return node;
    }
    else if(deep > 1) {
        node->next = FAddInTree(node->next, deep - 1, key, str);
        return node;
    }
    else {
        node->neighbor = FAddInTree(node->neighbor, deep, key, str);
        return node;
    }
}

TNode* FLoadTree(FILE* file, TNode* node, char* buffer) {
    unsigned long long key;
    int mem;
    int mover = 0;
    int step = 0;
    while(fscanf(file, "%s", buffer) != EOF) {
        step = 0;
        mover = 0;
        while(buffer[step] == '-') {
            buffer[step++] = '\0';
        }
        mem = step;
        while(buffer[step] != '\0') {
            buffer[mover] = buffer[step];
            step++;
            mover++;
        }
        if(buffer[mover - 1] == '$') {
            buffer[mover - 1] = '\0';
            buffer[mover] = '$';
            key = 0;
        }
        else {
            buffer[mover] = '\0';
            fscanf(file, "%llu", &key);
        }
        node = FAddInTree(node, mem, key, buffer);
    }
    return node;
}

void PrintTree(TNode* node, int deep) {
    if(node == NULL) {
        return;
    }
    else {
        for(int step = 0; step < deep; step++)
            printf("\t");
        printf("-%s\n", node->partOfWord);
        PrintTree(node->next, deep + 1);
        PrintTree(node->neighbor, deep);
        return;
    }
}

void FSaveTree(FILE* file, TNode* node, int deep) {
    if(node == NULL) {
        return;
    }
    else {
        for(int step = 0; step < deep; step++)
            fprintf(file, "-");
        fprintf(file, "-%s", node->partOfWord);
        if(node->partOfWord[strlen(node->partOfWord) + 1] == '$') {
            fprintf(file, "$\n");
        }
        else if(node->neighbor == NULL) {
            fprintf(file, " %llu\n", node->key);
        }
        else if(node->partOfWord[0] == '\0') {
            fprintf(file, " %llu\n", node->key);
        }
        FSaveTree(file, node->next, deep + 1);
        FSaveTree(file, node->neighbor, deep);
        return;
    }
}

void FindInTree(TNode* node, char* buffer, char* word, int step) {
    if(node == NULL) {
        return;
    }
    int tmp = 0;
    int mem1 = step;
    int mem2 = step;
    while(node->partOfWord[tmp] != '\0') {
        buffer[step] = node->partOfWord[tmp];
        step++;
        tmp++;
    }
    if(node->partOfWord[tmp + 1] == '$') {
        FindInTree(node->next, buffer, word, step);
    }
    else if(strcmp(buffer, word) == 0) {
        printf("OK: %llu\n", node->key);
        answer = 1;
    }
    while(buffer[mem1] != '\0') {
        buffer[mem1] = '\0';
        mem1++;
    }
    FindInTree(node->neighbor, buffer, word, mem2);
    return;
}

void StrToLower(char* str) {
    int step = 0;
    while(str[step] != '\0' && str[step] != EOF) {
        str[step] = tolower(str[step]);
        step++;
    }
    return;
}

TNode* AddInTree(TNode* node, char* str, unsigned long long int key) {
    if(node == NULL) {
        TNode* newNode = AddNode(key, str);
        return newNode;
    }
    else {
        int step = 0;
        while((str[step] == node->partOfWord[step]) && (str[step] != '\0' && node->partOfWord[step] != '\0')) {
            step++;
        }
        if(step == 0) {
            node->neighbor = AddInTree(node->neighbor, str, key);
            return node;
        }
        else if(node->partOfWord[step + 1] == '$') {
            node->next = AddInTree(node->next, str + step, key);
            return node;
        }
        else {
            char* tmp = malloc(sizeof(char)*(strlen(node->partOfWord) - step));
            strcpy(tmp, node->partOfWord + step);
            node->partOfWord = realloc(node->partOfWord, sizeof(char)*(step + 2));
            if(node->next != NULL) {
                node->next = AddAnotherNode(key, tmp, node->next);
            }
            else {
                node->next = AddInTree(node->next, tmp, key);
            }
            node->next = AddInTree(node->next, str + step, key);
            node->partOfWord[step] = '\0';
            node->partOfWord[step + 1] = '$';
            return node;
        }
    }
}

int main() {
    unsigned long long int key = 0;
    char pathToFile[256];
    char buffer[256];
    char str[256];
    FILE* file;
    TNode* root = NULL;
    for(;;) {
        scanf("%s", str);
        switch(str[0]) {
            case '\n':
                break;
            case '+':
                if(scanf("%s %llu", str, &key) == 2) {
                    StrToLower(str);
                    FindInTree(root, buffer, str, 0);
                    if(answer == 1) {
                        puts("Exist");
                        answer = 0;
                        break;
                    }
                    root = AddInTree(root, str, key);
                    PrintTree(root, 0);
                }
                else {
                    puts("Error: bad format!");
                    return 1;
                }
                break;
            case '-':
                if(scanf("%s %llu", str, &key) == 2) {
                    StrToLower(str);
                    //RemoveFromTree();
                }
                else {
                    puts("Error: bad format!");
                    return 1;
                }
                break;
            case '!':
                if(scanf("%s %s", str, pathToFile) == 2) {
                    if(strcmp(str,"Save") == 0) {
                        file = fopen(pathToFile, "w");
                        if(file == NULL) {
                            puts("Error: Can't open the file!");
                            return 1;
                        }
                        FSaveTree(file, root, 0);
                        puts("OK");
                        fclose(file);
                    }
                    else if(strcmp(str,"Load") == 0) {
                        file = fopen(pathToFile, "r");
                        if(file == NULL) {
                            puts("Error: Can't open the file!");
                            return 1;
                        }
                        ClearTree(root);
                        root = NULL;
                        root = FLoadTree(file, root, buffer);
                        puts("OK");
                    }
                    else {
                        puts("Error: bad format!");
                        return 1;
                    }
                }
                else {
                    puts("Error: bad format!");
                    return 1;
                }
                break;
            default:
                answer = 0;
                StrToLower(str);
                buffer[strlen(str)] = '\0';
                FindInTree(root, buffer, str, 0);
                if(answer == 0) {
                    puts("NoSuchWord");
                }
                break;
        }
    }
}
