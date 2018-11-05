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

TNode* FLoadTree(FILE* file, TNode* node, char* buffer) {
    unsigned long long int key;
    int step = 0;
    int mem;
    int check;
    char tmp;
    for(;;) {
        mem = step;
        step = 0;
        if (fread(&check, sizeof(int), 1, file)) {
            do {
                fread(&tmp, sizeof(char), 1, file);
                buffer[step] = tmp;
                ++step;
            } while(tmp != '#' && tmp != '$');
            if(tmp == '$') {
                key = 0;
                node = AddInTree(node, buffer, key);
            }
            else {
                fread(&key, sizeof(unsigned long long int), 1, file);
                node = AddInTree(node, buffer, key);
            }
        }
        else {
            break;
        }
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

void FSaveTree(FILE* file, TNode* node) {
    if(node == NULL) {
        return;
    }
    else {
        char tmp;
        int numb = 0;
        fwrite(&numb, sizeof(int), 1, file);
        fwrite(node->partOfWord, sizeof(char), strlen(node->partOfWord) + 1, file);
        if(node->partOfWord[strlen(node->partOfWord) + 1] == '$') {
          tmp = '$';
          fwrite(&tmp, sizeof(char), 1, file);
        }
        else {
          tmp = '#';
          fwrite(&tmp, sizeof(char), 1, file);
          fwrite(&node->key, sizeof(unsigned long long int), 1, file);
        }
        FSaveTree(file, node->next);
        FSaveTree(file, node->neighbor);
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
                        FSaveTree(file, root);
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
                        PrintTree(root, 0);
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
