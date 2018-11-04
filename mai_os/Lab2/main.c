#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double goodCount = 0;
double allCount = 0;
pthread_mutex_t mutex;

void foo(int s) {
    double try[2];
    pthread_mutex_lock(&mutex);
    for(int step = 0; step < s; ++step) {
        ++allCount;
        try[0] = rand() % 52;
        try[1] = rand() % 52;
        if(try[0] == 1 && try[1] == 1) {
            ++goodCount;
        }
    }
    printf("Check! Id of thread = %ld\n", pthread_self());
    pthread_mutex_unlock(&mutex);
    return;
}

void* potok(void* try) 
{
    int* countTry = (int*) try;
    foo(*countTry);
    pthread_exit(0);
}

int main(int argc, char* argv[])
{
	pthread_t tid; /* идентификатор потока */
	pthread_attr_t attr; /* отрибуты потока */
    int countM, countS;
    int ltime = time(NULL);
    unsigned int stime = (unsigned int) ltime / 2;
    srand(stime);
    pthread_mutex_init(&mutex, NULL);
    if (argc != 2) {
        fprintf(stderr,"usage: progtest <integer value>\n");
        return -1;
    }
    countM = atoi(argv[1]);
    if (countM < 0) {
        fprintf(stderr,"Аргумент %d не может быть отрицательным числом\n",atoi(argv[1]));
        return -1;
    }
    if ((countM % 2) == 0) {
        countM /= 2;
        countS = countM;
    } else {
        countM /= 2;
        countS = countM + 1;
    }
	pthread_attr_init(&attr);
	pthread_create(&tid,&attr,potok,&countS);
    foo(countM);
	pthread_join(tid,NULL);
    double answer = (goodCount / allCount) * 100;
    printf("Succesful attempts - %.0f\nAll attemps - %.0f\nChance = %f!\n", goodCount, allCount, answer);
    return 0;
}

