#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int answer = 0;

typedef struct Node {
    unsigned long long int  key;
    char*                   partOfWord;
    struct Node*            next;
    struct Node*            neighbor;
} TNode;

TNode* AddNode(unsigned long long int key, char* str, TNode* next, int mode) {
    TNode* tmp = (TNode*) malloc(sizeof(TNode));
    if(tmp == NULL) {
        puts("Error!");
        exit(0);
    }
    tmp->partOfWord = malloc(sizeof(char)*(strlen(str) + 2));
    strcpy(tmp->partOfWord, str);
    if(mode == 0) {
        tmp->key = key;
        tmp->partOfWord[strlen(str) + 1] = '\0';
    }
    else {
        tmp->key = 0;
        tmp->partOfWord[strlen(str) + 1] = '$';
    }
    tmp->next = next;
    tmp->neighbor = NULL;
    return tmp;
}

TNode* AddInTree(TNode* node, char* str, unsigned long long int key) {
    if(node == NULL) {
        TNode* newNode = AddNode(key, str, NULL, 0);
        return newNode;
    }
    else {
        int step = 0;
        while(str[step] == node->partOfWord[step] && str[step] != '\0' && node->partOfWord[step] != '\0') {
            step++;
        }
        if(step == 0) {
            node->neighbor = AddInTree(node->neighbor, str, key);
            return node;
        }        
        else if(node->partOfWord[step] == '\0' && node->partOfWord[step + 1] != '$' && str[step] == '\0') {
            puts("Exist");
            return node;
        }
        else if(node->partOfWord[step + 1] == '$') {
            node->next = AddInTree(node->next, str + step, key);
            return node;
        } 
        else {
            char* tmp = malloc(sizeof(char)*(strlen(node->partOfWord) - step + 1));
            strcpy(tmp, node->partOfWord + step);
            node->partOfWord = realloc(node->partOfWord, sizeof(char)*(step + 2));
            if(node->next != NULL) {
                node->next = AddNode(key, tmp, node->next, 1);
            }
            else {
                node->next = AddInTree(node->next, tmp, node->key);
            }
            free(tmp);
            node->next = AddInTree(node->next, str + step, key);
            node->key = 0;
            node->partOfWord[step] = '\0';
            node->partOfWord[step + 1] = '$';
            return node;
        }
    }
}

TNode* RemoveFromTree(TNode* node, char* word, int step) {
    if(node == NULL) {
        return NULL;
    }
    int tmp = 0;
    int begin = step;
    while(node->partOfWord[tmp] != '\0' && word[step] != '\0' && node->partOfWord[tmp] == word[step]) {
        step++;
        tmp++;
    }
    if(node->partOfWord[tmp + 1] == '$') {
        node->next = RemoveFromTree(node->next, word, step);
        if(node->next != NULL && node->next->neighbor == NULL && answer) {
            node->partOfWord = realloc(node->partOfWord, sizeof(char)*(strlen(node->partOfWord)+strlen(node->next->partOfWord) + 1));
            strcat(node->partOfWord, node->next->partOfWord);
            node->key = node->next->key;
            answer = 0;
            TNode* tmp = node->next->next;
            free(node->next->partOfWord);
            free(node->next);
            node->next = tmp;
        }
    }
    else if(word[step] == '\0' && node->partOfWord[tmp] == '\0') {
        printf("OK: %llu\n", node->key);
        answer = 1;
        TNode* save = node->neighbor;
        free(node->partOfWord);
        free(node);
        return save;
    }
    node->neighbor = RemoveFromTree(node->neighbor, word, begin);
    return node;
}

TNode* ClearTree(TNode* node) {
    if(node == NULL) {
        return node;
    }
    node->next = ClearTree(node->next);
    node->neighbor = ClearTree(node->neighbor);
    free(node->partOfWord);
    free(node);
    return NULL;
}

TNode* FLoadTree(FILE* file, TNode* node, char* buffer) {
    unsigned long long int key;
    int step = 0;
    unsigned char tmp;
    for(;;) {
        step = 0;
        if (fread(&tmp, sizeof(unsigned char), 1, file)) { //Проверяем, есть ли ещё кусок
            if(tmp != 219) {
                puts("Error: load failed!");
                answer = 1;
                return node;
            }
            do {
                fread(&tmp, sizeof(unsigned char), 1, file); //Посимвольно считываем и записываем в буффер
                buffer[step] = tmp;
                ++step;
            } while(tmp != 219);                    //Если встречаем конечный узел
            buffer[step] = '\0';
            fread(&key, sizeof(unsigned long long int), 1, file);
            node = AddInTree(node, buffer, key);
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
        printf("-%s", node->partOfWord);
        if(node->partOfWord[strlen(node->partOfWord) + 1] != '$') {
            printf(" = %llu\n", node->key);
        }
        else {
            printf("\n");
        }
        PrintTree(node->next, deep + 1);
        PrintTree(node->neighbor, deep);
        return;
    }
}

void FSaveTree(FILE* file, TNode* node, char* buffer, int step) {
    if(node == NULL) {
        return;
    }
    else {
        int begin = step;
        int mem = 0;
        while(node->partOfWord[mem] != '\0') {
            buffer[step] = node->partOfWord[mem];
            ++mem;
            ++step;
        }
        buffer[step] = '\0';
        if(node->partOfWord[mem + 1] == '$') {
            FSaveTree(file, node->next, buffer, step);
            if(node->neighbor != NULL) {
                FSaveTree(file, node->neighbor, buffer, begin);
            }
            return;
        }
        else {
            unsigned char tmp = 219;
            fwrite(&tmp, sizeof(unsigned char), 1, file);
            fwrite(buffer, sizeof(char), strlen(buffer) + 1, file);
            fwrite(&tmp, sizeof(unsigned char), 1, file);
            fwrite(&(node->key), sizeof(unsigned long long int), 1, file);
            printf("%s - %llu\n", buffer, node->key);
            if(node->neighbor != NULL) {
                FSaveTree(file, node->neighbor, buffer, begin);
            }
            return;
        }
    }
}

void FindInTree(TNode* node, char* word) {
    if(node == NULL) {
        return;
    }
    int step = 0;
    while(node->partOfWord[step] == word[step] && node->partOfWord[step] != '\0' && word[step] != '\0') {
        step++;
    }
    if(node->partOfWord[step + 1] == '$') {
        FindInTree(node->next, word + step);
    }
    else if(word[step] == '\0' && node->partOfWord[step] == '\0') {
        printf("OK: %llu\n", node->key);
        answer = 1;
        return;
    } 

    FindInTree(node->neighbor, word);
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
    char str[256];
    FILE* file;
    TNode* root = NULL;
    TNode* tmp = NULL;
    for(;;) {
        scanf("%s", str);
        switch(str[0]) {
            case '\n':
                break;
            case '+':
                if(scanf("%s%llu", str, &key) == 2) {
                    StrToLower(str);
                    root = AddInTree(root, str, key);
                }
                else {
                    puts("Error: bad format!");
                    return 1;
                }
                break;
            case '-':
                if(scanf("%s", str)) {
                    StrToLower(str);
                    RemoveFromTree(root, str, 0);
                    answer = 0;
                }
                else {
                    puts("Error: bad format!");
                    return 1;
                }
                break;
            case '!':
                if(scanf("%s", str) == 1) {
                    if(strcmp(str,"Save") == 0) {
                        scanf("%s", pathToFile);
                        file = fopen(pathToFile, "w");
                        if(file == NULL) {
                            puts("Error: Can't open the file!");
                            return 1;
                        }
                        FSaveTree(file, root, str, 0);
                        puts("OK");
                        fclose(file);
                    }
                    else if(strcmp(str,"Load") == 0) {
                        scanf("%s", pathToFile);
                        file = fopen(pathToFile, "r");
                        if(file == NULL) {
                            puts("Error: Can't open the file!");
                            return 1;
                        }
                        tmp = NULL;
                        tmp = FLoadTree(file, tmp, str);
                        if(answer == 1) {
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
                        puts("Error: wrong command!");
                        return 1;
                    }
                }
                else {
                    puts("Error: bad input format!");
                    return 1;
                }
                break;
            default:
                answer = 0;
                StrToLower(str);
                FindInTree(root, str);
                if(answer == 0) {
                    puts("NoSuchWord");
                }
                break;
        }
    }
}
