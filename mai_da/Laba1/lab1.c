#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

const int MAX_STRING_SIZE = 2048;

typedef struct TInfo {
    unsigned long long int number;
    int keyLength;
    char* key;
} TInfo;

typedef struct TBucket {
    unsigned long long int lowest;
    unsigned long long int largest;
    TInfo** data;
    int dataCount;
} TBucket;

//Перевод массива данных к массиву указателей для увеличения скрости работы программы

void OldToNew(TInfo** dataNew, TInfo* data, int dataCounter) {
    int counter;
    for (counter = 0; counter < dataCounter; counter++) {
        dataNew[counter] = &data[counter];
    }
    return;
}

//Создание бакетов, определение их границ

void MakeBucket(TBucket** bucket, int bucketsCounter, unsigned long long int min, 
                unsigned long long int max, unsigned long long int step, TBucket* createdBucket) {
    int counterBucket;
    for (counterBucket = 0; counterBucket < bucketsCounter - 1; counterBucket++) {
        createdBucket[counterBucket].dataCount = 0;
        createdBucket[counterBucket].lowest = min;
        createdBucket[counterBucket].largest = min + step - 1;
        createdBucket[counterBucket].data = NULL;
        bucket[counterBucket] = &createdBucket[counterBucket];
        min = min + step;
    }
    /*Поскольку верхней границей последнего бакета является 
     * максиальное полученное значение то он задаётся отдельно*/
    createdBucket[counterBucket].dataCount = 0;
    createdBucket[counterBucket].lowest = min;
    createdBucket[counterBucket].largest = max;
    createdBucket[counterBucket].data = NULL;
    bucket[counterBucket] = &createdBucket[counterBucket];
    return;
}

//Заполнение бакетов массивом указателей

void FullBuckets(TBucket** bucket, int bucketsCounter, TInfo** dataNew, int dataCount) {
    int counterBucket, counterData;
    for (counterData = 0; counterData < dataCount; counterData++) {
        for (counterBucket = 0; counterBucket < bucketsCounter; counterBucket++) {
            if (dataNew[counterData]->number >= bucket[counterBucket]->lowest 
                && dataNew[counterData]->number <= bucket[counterBucket]->largest) {
                bucket[counterBucket]->data = (TInfo**)realloc(bucket[counterBucket]->data, (bucket[counterBucket]->dataCount + 1) * sizeof(TInfo*));
                bucket[counterBucket]->data[bucket[counterBucket]->dataCount] = dataNew[counterData];
                bucket[counterBucket]->dataCount++;
                break;
            }
        }
    }
    return;
}

/*Сортировка пузырьком одного бакета с последующей печатью 
 * элементов в этом бакете и освобождением массива data*/

void BucketSortAndPrint(TBucket** bucket, int bucketsCounter) {
    int startBucket, dataStart, printCount, keyCount;
    TInfo* tmpData;
    for (startBucket = 0; startBucket < bucketsCounter; startBucket++) {
        if (bucket[startBucket]->data != NULL && bucket[startBucket] != NULL) {
            printCount = bucket[startBucket]->dataCount;
            while (bucket[startBucket]->dataCount != 1) {
                for (dataStart = 0; dataStart < bucket[startBucket]->dataCount - 1; dataStart++) {
                    if (bucket[startBucket]->data[dataStart]->number > bucket[startBucket]->data[dataStart + 1]->number) {
                        tmpData = bucket[startBucket]->data[dataStart + 1];
                        bucket[startBucket]->data[dataStart + 1] = bucket[startBucket]->data[dataStart];
                        bucket[startBucket]->data[dataStart] = tmpData;
                    }
                }
                bucket[startBucket]->dataCount--;
            }
            for (dataStart = 0; dataStart < printCount; dataStart++) {//Печать значения и ключа
                printf("%llu\t", bucket[startBucket]->data[dataStart]->number);
                for (keyCount = 0; keyCount < bucket[startBucket]->data[dataStart]->keyLength; keyCount++) {
                    printf("%c", bucket[startBucket]->data[dataStart]->key[keyCount]);
                }
                printf("\n");
                free(bucket[startBucket]->data[dataStart]->key);
            }
            free(bucket[startBucket]->data);
        }
    }
    return;
}

int main() {
    time_t start, end;
    start = clock();
    TInfo* data = NULL;
    char preDataKey[MAX_STRING_SIZE];
    unsigned long long int preDataNumber;
    preDataKey[0] = EOF;
    scanf("%llu%s", &preDataNumber, preDataKey);
    if (preDataKey[0] == EOF) {
        return 0;
    }
    int keyLong;
    keyLong = strlen(preDataKey);
    data = (TInfo*) realloc(data, sizeof(TInfo));
    data[0].key = (char*) malloc(sizeof(char) * keyLong);
    data[0].keyLength = keyLong;
    data[0].number = preDataNumber;
    strncpy(data[0].key, preDataKey, keyLong);
    int dataCounter = 1;
    unsigned long long int min = data[0].number;
    unsigned long long int max = data[0].number;
    while (preDataKey[0] != EOF) {//Цикл считывания входных данных
        preDataKey[0] = EOF;
        scanf("%llu%s", &preDataNumber, preDataKey);
        if (preDataKey[0] == EOF) {//Выход из цикла получения данных
            break;
        }
        data = (TInfo*) realloc(data, sizeof(TInfo) * (dataCounter + 1));
        keyLong = strlen(preDataKey);
        data[dataCounter].key = (char*) malloc(sizeof(char) * keyLong);
        data[dataCounter].keyLength = keyLong;
        data[dataCounter].number = preDataNumber;
        strncpy(data[dataCounter].key, preDataKey, keyLong);
        if (data[dataCounter].number > max) {
            max = data[dataCounter].number;
        }
        if (data[dataCounter].number < min) {
            min = data[dataCounter].number;
        }
        dataCounter++;
    }
    if (min == max) {
        int counter = 0;
        while (counter < dataCounter) {
            printf("%llu\t", data[counter].number);
            for (keyLong = 0; keyLong < data[counter].keyLength; keyLong++) {//Печать строки переменной длинны
                printf("%c", data[counter].key[keyLong]);
            }
            free(data[counter].key);
            counter++;
            printf("\n");
        }
        free(data);
        //end = clock();
        //printf("Время = %.5f\n", ((double) end - start)/((double) CLOCKS_PER_SEC));
        return 0;
    }
    int bucketsCounter = dataCounter;
    unsigned long long int step = (max - min + 1) / bucketsCounter;
    TInfo** dataNew = NULL;
    dataNew = (TInfo**) malloc(sizeof(TInfo*) * dataCounter);
    OldToNew(dataNew, data, dataCounter);
    TBucket** bucket = (TBucket**) malloc(sizeof(TBucket*) * bucketsCounter);
    TBucket* createdBucket = (TBucket*) malloc(sizeof(TBucket) * bucketsCounter);
    MakeBucket(bucket, bucketsCounter, min, max, step, createdBucket);
    FullBuckets(bucket, bucketsCounter, dataNew, dataCounter);
    BucketSortAndPrint(bucket, bucketsCounter);
    free(data);
    free(dataNew);
    free(createdBucket);
    free(bucket);
    end = clock();
    printf("Время = %.5f\n", ((double) end - start)/((double) CLOCKS_PER_SEC));
    return 0;
}
