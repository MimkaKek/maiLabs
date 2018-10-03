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
    unsigned long long int lower_border;
    unsigned long long int upper_border;
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
 *     - "Цепочку" из указателей векторов Node;
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
 * Функция Create_Buckets берёт на вход:
 *     - Массив указателей на контейнеры Bucket;
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

void Create_Buckets(Bucket** array, unsigned int *amount, unsigned long long int step, unsigned long long int beginning, unsigned long long int *maxkey) {
    for(int number = 0; number < *amount; number++) {
        Bucket* newBucket = (Bucket*)malloc(sizeof(Bucket));
        if(newBucket == NULL) {
            puts("Error: недостаточно памяти!");
            exit(0);
        }
        array[number] = newBucket;
        newBucket->counter = 0;
        newBucket->lower_border = beginning;
        newBucket->upper_border = beginning + step - 1;
        newBucket->start = NULL;
        beginning = beginning + step;
    }
    array[(*amount) - 1]->upper_border = *maxkey;
    return;
}

/* 
 * Функция Add_Node берёт на вход:
 *     - "Цепочку" из указателей векторов Node;
 *     - Указатель на ключ key;
 *     - Указатель на значение str;
 * 
 * В конец "цепочки" из указателей векторов добавляет новый указатель на новый вектор.
 *
 */

Vector* Add_Node(Vector* node, unsigned long long int* key, unsigned long long int* str) {
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
        node->next = Add_Node(node->next, key, str);
        return node;
    }
}

/* 
 * Функция Add_In_Bucket берёт на вход:
 *     - "Цепочку" из указателей векторов Node, которая закрепляна за контейнером;
 *     - Указатель current на тот вектор, что хотим добавить;
 * 
 * Функция добавляет в конец к "цепочке" векторов следующий вектор.           
 *
 */

Vector* Add_In_Bucket(Vector* node, Vector* current) {
    if(node == NULL) {
        node = current;
        node->next = NULL;
    }
    else {
        node->next = Add_In_Bucket(node->next, current);
    }
    return node;
}

/* 
 * Функция Place_In_Buckets берёт на вход:
 *     - Массив указателей на контейнеры array_of_buckets;
 *     - Массив указателей на векторы array_of_vectors;
 *     - Указатель на количество контейнеров;
 *     - Указатель на количество векторов;
 * 
 * Сортирует вектора по контейнера в соответствии с их ключами.
 *
 */

void Place_In_Buckets(Bucket** array_of_buckets, Vector** array_of_vectors, unsigned int* amount_of_buckets, unsigned int* counter) {
    for(int numb_vector = 0; numb_vector < *counter; numb_vector++) {
        for(int numb_bucket = 0; numb_bucket < *amount_of_buckets; numb_bucket++) {
            if(array_of_vectors[numb_vector]->key >= array_of_buckets[numb_bucket]->lower_border 
               && array_of_vectors[numb_vector]->key <= array_of_buckets[numb_bucket]->upper_border) {
                array_of_buckets[numb_bucket]->start = Add_In_Bucket(array_of_buckets[numb_bucket]->start, array_of_vectors[numb_vector]);
                array_of_buckets[numb_bucket]->counter++;
                break;
            }
        }
    }
    return;
}

/* 
 * Функция Sort_Vector берёт на вход:
 *     - "Цепочку" из указателей векторов Node, закреплённых за контейнером;
 *     - Число указателей на вектора counter, которые находятся в контейнере;
 * 
 * Организует сортировку. Временно создаёт массив указателей и "привязывает" его
 * Linker'ом. Производит сортировку вставкой, которая эффективна при малом количестве
 * элементов массива (что мы гарантированно получаем при использовании карманной сортировки).
 * "На месте" выводит результат. Освобождает использованную память.
 *
 */

void Sort_Vector(Vector* node, int counter) {
    Vector** array_of_vectors = (Vector**) malloc(sizeof(Vector*)*counter);
    if(array_of_vectors == NULL) {
        puts("Error: недостаточно памяти");
        exit(0);
    }
    Vector* tmp;
    Linker(node, counter, array_of_vectors);
    for(int sorted_part = 1; sorted_part < counter; sorted_part++) { 
        for(int step = sorted_part; step > 0 && array_of_vectors[step - 1]->key > array_of_vectors[step]->key; step--) {
            tmp = array_of_vectors[step - 1];
            array_of_vectors[step - 1] = array_of_vectors[step];
            array_of_vectors[step] = tmp;
        }
    }
    for(int step = 0; step < counter; step++) {
        printf("%llu\t%llu\n", array_of_vectors[step]->key, array_of_vectors[step]->str);
        free(array_of_vectors[step]);
    }
    free(array_of_vectors);
    return;
}

/* 
 * Функция Sort_Bucket берёт на вход:
 *     - Массив указателей на контейнеры array;
 *     - Указатель на количество контейнеров amount;
 * 
 * Пробегает по массиву указателей на контейнеры. Если контейнер
 * пустой - переходит к следующему. Иначе - производит сортирку
 * внутри этого контейнера.
 *
 */
 
void Sort_Bucket(Bucket** array, unsigned int* amount) {
    for(int step = 0; step < *amount; step++) {
        if(array[step]->start != NULL) {
            Sort_Vector(array[step]->start, array[step]->counter);
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
    bool first_vector = true;
    bool read_numb = true; //Считывать ключ
    bool skip_n = true;
    Vector* root = NULL;
    Vector* current = NULL;
    Vector* previous = NULL;
    //FILE* file = fopen("test","r");
    do {
        //tmp = fgetc(file);
        tmp = getchar();
        if(tmp >= '0' && tmp <= '9' && read_numb) {
            skip_n = false;
            key = (unsigned long long int) key * 10 + Atoi(tmp);
        }
        else if(!read_numb && tmp >= '0' && tmp <= '9') {
            value = (unsigned long long int) value * 10 + Atoi(tmp);
        }
        else if(tmp == '\t' || tmp == ' ') {
            read_numb = false;
        }
        else if(tmp == '\n' && !skip_n) {
            read_numb = true;
            skip_n = true;
            if(first_vector) {
                first_vector = false;
                maxkey = key;
                minkey = key;
                root = Add_Node(root, &key, &value);
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
        else if(tmp == '\n' && skip_n) {
            read_numb = true;
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
    unsigned int amount_of_buckets = (unsigned int) sqrt(counter);
    unsigned long long int width_of_buckets = (maxkey - minkey) / amount_of_buckets;

    Vector** array_of_vectors = (Vector**) malloc(sizeof(Vector*)*counter);
    if(array_of_vectors == NULL) {
        puts("Error: недостаточно памяти");
        return 0;
    }
    Bucket** array_of_buckets = (Bucket**) malloc(sizeof(Bucket*)*amount_of_buckets);
    if(array_of_buckets == NULL) {
        puts("Error: недостаточно памяти");
        return 0;
    }
    Linker(root, counter, array_of_vectors);
    Create_Buckets(array_of_buckets, &amount_of_buckets, width_of_buckets, minkey, &maxkey);
    Place_In_Buckets(array_of_buckets, array_of_vectors, &amount_of_buckets, &counter);
    free(array_of_vectors);
    Sort_Bucket(array_of_buckets, &amount_of_buckets);
    //Print_Buckets(array_of_buckets, &amount_of_buckets);
    free(array_of_buckets);
    //end = clock();
    //printf("Время = %.4f\n", ((double) end - start)/((double) CLOCKS_PER_SEC));
    return 0;
}
