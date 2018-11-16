#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

unsigned long long int goodCount = 0;
//unsigned long long int allCount = 0;
pthread_mutex_t mutex;

int CalculateChance(int s) {
    //pthread_mutex_lock(&mutex);
    unsigned long long int goodAttempt = 0;
    double card[2];
    for(int step = 0; step < s; ++step) {
        //pthread_mutex_lock(&mutex);
        //++allCount;
        //pthread_mutex_unlock(&mutex);
        card[0] = rand() % 52;
        card[1] = rand() % 52;
        if(card[0] == card[1]) {
            ++goodAttempt;
        }
    }
    //printf("Check! Id of thread = %ld\n", pthread_self());
    //pthread_mutex_unlock(&mutex);
    return goodAttempt;
}

void* InputForThread(void* try) 
{
    int* countTry = (int*) try;
    goodCount += CalculateChance(*countTry);
    pthread_exit(0);
}

int main(int argc, char* argv[]) {
    pthread_attr_t attr;
    time_t start, end;
    start = clock();
    int countM, countS, numberOfThreads;
    int ltime = time(NULL);
    unsigned int stime = (unsigned int) ltime / 2;
    srand(stime);
    errno = pthread_mutex_init(&mutex, NULL);
    if( errno != 0 ) {
        fprintf(stderr, "Error: mutex init failed.\n");
    }
    if (argc != 3) {
        fprintf(stderr,"usage: ./start.exec <number of attempts> <number of threads>\n");
        exit(1);
    }
    countM = atoi(argv[1]);
    numberOfThreads = atoi(argv[2]);
    pthread_t* id = malloc(sizeof(pthread_t)*(numberOfThreads - 1));
    if (countM <= 0 || numberOfThreads <= 0) {
        fprintf(stderr,"Error: value can't be less or equal zero\n");
        exit(1);
    }
    countS = countM / numberOfThreads;
    pthread_attr_init(&attr);
    CalculateChance(countS + (countM % numberOfThreads));
    for(int i = 0; i < numberOfThreads - 1; ++i) {
        errno = pthread_create(id + i, &attr, InputForThread, &countS);
        if( errno == EAGAIN ) {
            fprintf(stderr, "Error: a system limit is exceeded.\n");
            exit(1);
        }
        else if ( errno == EINVAL ) {
            fprintf(stderr, "Error: the value of attr_t is invalid.\n");
            exit(1);
        }
        else if ( errno == EPERM ) {
            fprintf(stderr, "Error: the caller does not have appropriate permission to set the required scheduling parameters or scheduling policy.\n");
            exit(1);
        }
    }
    for(int i = 0; i < numberOfThreads - 1; ++i) {
        errno = pthread_join(id[i], NULL);
        if ( errno == ESRCH ) {
            fprintf(stderr, "Error: no thread could be found corresponding to that specified by the given thread ID.\n");
            exit(1);
        }
        else if ( errno == EDEADLK ) {
            fprintf(stderr, "Error: a deadlock was detected or the value of thread specifies the calling thread.\n");
            exit(1);
        }
        else if ( errno == EINVAL ) {
            fprintf(stderr, "Error: the value specified by thread does not refer to a joinable thread.\n");
            exit(1);
        }
    }
    /*if(allCount < atoi(argv[1])) {
        fprintf(stderr, "Error: finished attempts less than specified attempts\n");
    }*/
    double answer = ((double) goodCount / (double) atoi(argv[1])) * 100;
    printf("Succesful attempts - %llu\nAll attemps - %llu\nChance = %.4f%%\n", goodCount, /*allCount*/(unsigned long long int) atoi(argv[1]), answer);
    end = clock();
    printf("Time = %.5f\n", ((double) end - start)/((double) CLOCKS_PER_SEC));
    free(id);
    return 0;
}

