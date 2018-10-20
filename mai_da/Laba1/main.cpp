#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>

/* 
 * Объявление двух структур:
 *     векторы (TVector);
 *     контейнеры (TBucket). 
 * 
 */
 
typedef struct TVector {
    unsigned long long int key;
    unsigned long long int str;
    struct TVector* next;
} Vector;

typedef struct TBucket {
    unsigned long long int lowerBorder;
    unsigned long long int upperBorder;
    unsigned int counter;
    struct TVector* start;
} Bucket;

/* Функции нахождения максимума минимума соответственно */

unsigned long int Max(unsigned long long int a,unsigned long long int b) {
    if(a >= b) {
        return a;
    }
    else {
        return b;
    }
}

unsigned long int Min(unsigned long long int a,unsigned long long int b) {
    if(a <= b) {
        return a;
    }
    else {
        return b;
    }
}

/* 
 * Функция Linker берёт на вход:
 *     - "Цепочку" из указателей векторов node;
 *     - Число векторов для массива указателей array;
 *     - Массив указателей array;
 * 
 * "Связывает" массив указателей по элементно к "цепочке" векторов
 * для дальнейшей более конфортной и быстрой работы с ними.                
 *
 */
 

void Linker(Vector* node, unsigned int counter, Vector** array) {
    unsigned int step = 0;
    while(node != NULL && step < counter) {
        array[step] = node;
        node = node->next;
        step++;
    }
    return;
}

/* 
 * Функция CreateBuckets берёт на вход:
 *     - Массив указателей array на контейнеры;
 *     - Указатель на количество этих указателей amount;
 *     - Шаг для границ контейнера step;
 *     - Нижнюю границу (минимальную) beginning;
 *     - Указатель на максимальное значение maxkey;
 * 
 * Фукция создаёт amount контейнеров и "закрепляет" их за массивом.
 * Контейнеры настраиваются там же в соответствии с заданными параметрами.
 * Для последнего контейнера берёт верхнюю границу *maxkey во избежание 
 * возможных неточностей.
 *
 */

void CreateBuckets(Bucket** array, unsigned int *amount, unsigned long long int step, unsigned long long int beginning, unsigned long long int *maxkey) {
    for(int number = 0; number < *amount; number++) {
        Bucket* newBucket = (Bucket*)malloc(sizeof(Bucket));
        if(newBucket == NULL) {
            puts("Error: недостаточно памяти!");
            exit(0);
        }
        array[number] = newBucket;
        newBucket->counter = 0;
        newBucket->lowerBorder = beginning;
        newBucket->upperBorder = beginning + step - 1;
        newBucket->start = NULL;
        beginning = beginning + step;
    }
    array[(*amount) - 1]->upperBorder = *maxkey;
    return;
}

/* 
 * Функция AddNode берёт на вход:
 *     - "Цепочку" из указателей векторов node;
 *     - Указатель на ключ key;
 *     - Указатель на значение str;
 * 
 * В конец "цепочки" из указателей векторов добавляет новый указатель на новый вектор.
 *
 */

Vector* AddNode(Vector* node, unsigned long long int* key, unsigned long long int* str) {
    if(node == NULL) {
        Vector* newNode = (Vector*)malloc(sizeof(Vector));
        if(newNode == NULL) {
            puts("Error: Нехватка памяти! Завершение программы");
            exit (0);
        }
        newNode->str = *str;
        newNode->key = *key;
        newNode->next = NULL;
        return newNode;
    }
    else {
        node->next = AddNode(node->next, key, str);
        return node;
    }
}

/* 
 * Функция AddInBucket берёт на вход:
 *     - "Цепочку" из указателей векторов node, которая закрепляна за контейнером;
 *     - Указатель current на тот вектор, что хотим добавить;
 * 
 * Функция добавляет в конец к "цепочке" векторов следующий вектор.           
 *
 */

Vector* AddInBucket(Vector* node, Vector* current) {
    if(node == NULL) {
        node = current;
        node->next = NULL;
    }
    else {
        node->next = AddInBucket(node->next, current);
    }
    return node;
}

/* 
 * Функция PlaceInBuckets берёт на вход:
 *     - Массив указателей на контейнеры arrayOfBuckets;
 *     - Массив указателей на векторы arrayOfVectors;
 *     - Указатель на количество контейнеров amountOfBuckets;
 *     - Указатель на количество векторов counter;
 * 
 * Сортирует вектора по контейнера в соответствии с их ключами.
 *
 */

void PlaceInBuckets(Bucket** arrayOfBuckets, Vector** arrayOfVectors, unsigned int* amountOfBuckets, unsigned int* counter) {
    for(int numbVector = 0; numbVector < *counter; numbVector++) {
        for(int numbBucket = 0; numbBucket < *amountOfBuckets; numbBucket++) {
            if(arrayOfVectors[numbVector]->key >= arrayOfBuckets[numbBucket]->lowerBorder 
               && arrayOfVectors[numbVector]->key <= arrayOfBuckets[numbBucket]->upperBorder) {
                arrayOfBuckets[numbBucket]->start = AddInBucket(arrayOfBuckets[numbBucket]->start, arrayOfVectors[numbVector]);
                arrayOfBuckets[numbBucket]->counter++;
                break;
            }
        }
    }
    return;
}

/* 
 * Функция SortVector берёт на вход:
 *     - "Цепочку" из указателей векторов Node, закреплённых за контейнером;
 *     - Число указателей на вектора counter, которые находятся в контейнере;
 * 
 * Организует сортировку. Временно создаёт массив указателей и "привязывает" его
 * Linker'ом. Производит сортировку вставкой, которая эффективна при малом количестве
 * элементов массива (что мы гарантированно получаем при использовании карманной сортировки).
 * "На месте" выводит результат. Освобождает использованную память.
 *
 */

void SortVector(Vector* node, int counter) {
    Vector** arrayOfVectors = (Vector**) malloc(sizeof(Vector*)*counter);
    if(arrayOfVectors == NULL) {
        puts("Error: недостаточно памяти");
        exit(0);
    }
    Vector* tmp;
    Linker(node, counter, arrayOfVectors);
    for(int sortedPart = 1; sortedPart < counter; sortedPart++) { 
        for(int step = sortedPart; step > 0 && arrayOfVectors[step - 1]->key > arrayOfVectors[step]->key; step--) {
            tmp = arrayOfVectors[step - 1];
            arrayOfVectors[step - 1] = arrayOfVectors[step];
            arrayOfVectors[step] = tmp;
        }
    }
    for(int step = 0; step < counter; step++) {
        printf("%llu\t%llu\n", arrayOfVectors[step]->key, arrayOfVectors[step]->str);
        free(arrayOfVectors[step]);
    }
    free(arrayOfVectors);
    return;
}

/* 
 * Функция SortBucket берёт на вход:
 *     - Массив указателей на контейнеры array;
 *     - Указатель на количество контейнеров amount;
 * 
 * Пробегает по массиву указателей на контейнеры. Если контейнер
 * пустой - переходит к следующему. Иначе - производит сортирку
 * внутри этого контейнера.
 *
 */
 
void SortBucket(Bucket** array, unsigned int* amount) {
    for(int step = 0; step < *amount; step++) {
        if(array[step]->start != NULL) {
            SortVector(array[step]->start, array[step]->counter);
        }
        free(array[step]);
    }
    return;
}

/*  На память...
 * 
void Print_Buckets(Bucket** array_of_buckets, unsigned int* amount) {
    Vector* tmp = NULL;
    Vector* prev = NULL;
    for(int step = 0; step < *amount; step++) {
        if(array_of_buckets[step]->start != NULL) {
            tmp = array_of_buckets[step]->start;
            while(tmp != NULL) {
                printf("%llu\t%llu\n", tmp->key, tmp->str);
                prev = tmp;
                tmp = tmp->next;
                free(prev);
            }
        }
        free(array_of_buckets[step]);
    }
    return;
}
* */

/* 
 * Функция Atoi берёт на вход:
 *     - символ tmp;
 * 
 * Преобразует символ char в unsigned long long int.
 *
 */

unsigned long long int Atoi(char tmp) {
    switch(tmp) {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
    }
    return 0;
}

/* 
 * Функция main производит чтение потока ввода,
 * производит его обработку и создаёт вектора. 
 * Далее объявляет функции выше, которые описаны
 * выше.
 * 
 */

int main() {
    
    //time_t start, end;
    //start = clock();
    unsigned int counter = 0;
    unsigned long long int key = 0;
    unsigned long long int minkey = 0;
    unsigned long long int maxkey = 0;
    unsigned long long int value = 0;
    char tmp = '0';
    bool firstVector = true;
    bool readNumb = true; //Считывать ключ 10k = 0.0414 sec 20k = 0.0808 sec 40k = 0.1908 80k = 0.4047 sec 200k = 1.3766 sec
    bool skipN = true;
    Vector* root = NULL;
    Vector* current = NULL;
    Vector* previous = NULL;
    //FILE* file = fopen("test","r");
    do {
        //tmp = fgetc(file);
        tmp = getchar();
        if(tmp >= '0' && tmp <= '9' && readNumb) {
            skipN = false;
            key = (unsigned long long int) key * 10 + Atoi(tmp);
        }
        else if(!readNumb && tmp >= '0' && tmp <= '9') {
            value = (unsigned long long int) value * 10 + Atoi(tmp);
        }
        else if(tmp == '\t' || tmp == ' ') {
            readNumb = false;
        }
        else if(tmp == '\n' && !skipN) {
            readNumb = true;
            skipN = true;
            if(firstVector) {
                firstVector = false;
                maxkey = key;
                minkey = key;
                root = AddNode(root, &key, &value);
                previous = root;
                key = 0;
                value = 0;
                counter = 1;
                continue;
            }
            current = (Vector*)malloc(sizeof(Vector));
            if(current == NULL) {
                puts("Error: Нехватка памяти! Завершение программы");
                exit (0);
            }

            previous->next = current;
            current->str = value;
            current->key = key;
            current->next = NULL;

            previous = current;
            current = current->next;

            minkey = Min(key, minkey);
            maxkey = Max(key, maxkey);

            key = 0;
            value = 0;

            counter++;
        }
        else if(tmp == '\n' && skipN) {
            readNumb = true;
        }
        else if((key == 0 || value == 0) && tmp == '-') {
            printf("Error: отрицательное число! Строка - %d\n", counter + 1);
            return 0;
        }
        else if(tmp != '\377'){
            printf("Error: неверный формат! Строка - %d\n", counter + 1);
            return 0;
        }
    } while(tmp != '\0' && tmp != EOF);

    if(counter == 0) {
        return 0;
    }
    //fclose(file);
    unsigned int amountOfBuckets = (unsigned int) sqrt(counter);
    unsigned long long int widthOfBuckets = (maxkey - minkey) / amountOfBuckets;

    Vector** arrayOfVectors = (Vector**) malloc(sizeof(Vector*)*counter);
    if(arrayOfVectors == NULL) {
        puts("Error: недостаточно памяти");
        return 0;
    }
    Bucket** arrayOfBuckets = (Bucket**) malloc(sizeof(Bucket*)*amountOfBuckets);
    if(arrayOfBuckets == NULL) {
        puts("Error: недостаточно памяти");
        return 0;
    }
    Linker(root, counter, arrayOfVectors);
    CreateBuckets(arrayOfBuckets, &amountOfBuckets, widthOfBuckets, minkey, &maxkey);
    PlaceInBuckets(arrayOfBuckets, arrayOfVectors, &amountOfBuckets, &counter);
    free(arrayOfVectors);
    SortBucket(arrayOfBuckets, &amountOfBuckets);
    //Print_Buckets(array_of_buckets, &amount_of_buckets);
    free(arrayOfBuckets);
    //end = clock();
    //printf("Время = %.4f\n", ((double) end - start)/((double) CLOCKS_PER_SEC));
    return 0;
}
