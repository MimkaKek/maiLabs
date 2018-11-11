#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double goodCount = 0;
double allCount = 0;
pthread_mutex_t mutex;

void foo(int s) {
    pthread_mutex_lock(&mutex);
    double try[2];
    for(int step = 0; step < s; ++step) {
        ++allCount;
        try[0] = rand() % 52;
        try[1] = rand() % 52;
        if(try[0] == 1 && try[1] == 1) {
            ++goodCount;
        }
    }
    //printf("Check! Id of thread = %ld\n", pthread_self());
    pthread_mutex_unlock(&mutex);
    return;
}

void* potok(void* try) 
{
    int* countTry = (int*) try;
    foo(*countTry);
    pthread_exit(0);
}

int main(int argc, char* argv[]) {
	pthread_attr_t attr; /* отрибуты потока */
    int countM, countS, numberOfThreads;
    int ltime = time(NULL);
    unsigned int stime = (unsigned int) ltime / 2;
    srand(stime);
    pthread_mutex_init(&mutex, NULL);
    if (argc != 3) {
        fprintf(stderr,"usage: ./start.exec <number of attempts> <number of threads>\n");
        return -1;
    }
    countM = atoi(argv[1]);
    numberOfThreads = atoi(argv[2]);
    pthread_t* id = malloc(sizeof(pthread_t)*(numberOfThreads - 1));
    if (countM <= 0 || numberOfThreads <= 0) {
        fprintf(stderr,"Error: value can't be less or equal zero\n");
        return -1;
    }
    countS = countM / numberOfThreads;
    pthread_attr_init(&attr);
    foo(countS + (countM % numberOfThreads));
    for(int i = 0; i < numberOfThreads - 1; ++i) {
        pthread_create(id + i,&attr,potok,&countS);
    }
    for(int i = 0; i < numberOfThreads - 1; ++i) {
        pthread_join(id[i],NULL);
    }
    if(allCount < atoi(argv[1])) {
        fprintf(stderr,"Error: something went wrong\n");
        return -1;
    }
    double answer = (goodCount / allCount) * 100;
    printf("Succesful attempts - %.0f\nAll attemps - %.0f\nChance = %.4f%%\n", goodCount, allCount, answer);
    free(id);
    return 0;
}

