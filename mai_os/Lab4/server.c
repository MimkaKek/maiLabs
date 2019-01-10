#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "zmq.h"

char                            adress[] = {'t','c','p',':','/','/','*',':','0','0','0','0', '\0'};

int                             CARD = 0;
int                             LOGIN = 1;
int                             CONTINUE = 1;
int                             GO = 1;

pthread_mutex_t                 CV_MUTEX;
pthread_mutexattr_t             ATTR_CV_MUTEX;
pthread_cond_t                  COND_VAR;
pthread_condattr_t              ATTR_COND_VAR;

typedef struct MTS {
        char                    login[128];
        char                    number[20];
        char                    aimNumb[20];
        char                    aimLogin[128];
        int                     act;
        double                  value;
} MessageToServer;

typedef struct MFS {
        int                     status;
        char                    number[20];
        double                  value;
} MessageFromServer;

typedef struct USR {
        double                  key;
        char                    login[128];
        char                    number[20];
        double                  value;
} TUser;

typedef struct TVector {
        TUser*                  users;
        int                     current;
        int                     size;
        pthread_mutex_t         mutex;
} TVector;

TVector*                        vector;

void SendMessage(MessageFromServer* msg, void* serverSocket) {
        zmq_msg_t zmqMessage;
        zmq_msg_init_size(&zmqMessage, sizeof(MessageFromServer));
        memcpy(zmq_msg_data(&zmqMessage), msg, sizeof(MessageFromServer));
        zmq_sendmsg(serverSocket, &zmqMessage, 0);
        zmq_msg_close(&zmqMessage);
        return;
}

void RecvMessage(void* serverSocket, MessageToServer* recv) {
        zmq_msg_t reply;
        zmq_msg_init(&reply);
        zmq_msg_recv(&reply, serverSocket, 0);
        MessageToServer* tmp1 = (MessageToServer*) zmq_msg_data(&reply);
        
        recv->act = tmp1->act;
        recv->value = tmp1->value;
        memcpy(recv->login, tmp1->login, sizeof(char)*128);
        memcpy(recv->number, tmp1->number, sizeof(char)*20);
        memcpy(recv->aimLogin, tmp1->aimLogin, sizeof(char)*128);
        memcpy(recv->aimNumb, tmp1->aimNumb, sizeof(char)*20);
        
        zmq_msg_close(&reply);
        return;
}

void GetPort() {
        
        system("clear");
        printf("Enter the port. You need 4 numbers.\n");
        
        char            menu[5];
        int             error = 1;
        int             counter = 0;
        
        while(error) {
                printf("> ");
                scanf("%s", menu);
                counter = 0;
                for(int i = 0; i < 4; ++i) {
                        if(menu[i] >= '0' && menu[i] <= '9') {
                                adress[8 + i] = menu[i];
                                ++counter;
                        }
                        else {
                                break;
                        }
                }
                if(counter == 4 && (menu[4] == '\n' || menu[4] == '\0')) {
                        error = 0;
                }
                else {
                        printf("Wrong input. Try again.\n");
                }
        }
}

int FindInVectorNumber(char* number) {

        for(int i = 0; i < vector->current; ++i) {
                if(strcmp(number, vector->users[i].number) == 0) {
                        return i;
                }
        }
        
        return -1;
}

int FindInVectorLogin(char* login) {

        for(int i = 0; i < vector->current; ++i) {
                if(strcmp(login, vector->users[i].login) == 0) {
                        return i;
                }
        }
        
        return -1;
}

TVector* AddInVector(char* login, char* number) {
        pthread_mutex_lock(&(vector->mutex));
        if(vector->current == 0) {
                vector->current = 1;
                vector->size = 1;
                vector->users = (TUser*) malloc(sizeof(TUser));
                if(!vector->users) {
                        printf("ERROR: bad malloc!\n");
                        exit(EXIT_FAILURE);
                }
                strcpy(vector->users[0].login, login);
                strcpy(vector->users[0].number, number);
                vector->users[0].value = 0;
                vector->users[0].key = 0;
        }
        else {
                if(vector->current == vector->size) {
                        vector->size *= 2;
                        vector->users = (TUser*) realloc(vector->users, sizeof(TUser)*vector->size);
                        if(!vector->users) {
                                printf("ERROR: bad realloc!\n");
                                exit(EXIT_FAILURE);
                        }
                }
                vector->current += 1;
                strcpy(vector->users[vector->current - 1].login, login);
                strcpy(vector->users[vector->current - 1].number, number);
                vector->users[vector->current - 1].value = 0;
                vector->users[vector->current - 1].key = 0;
        }
        pthread_mutex_unlock(&(vector->mutex));
        return vector;
}

void Registrate(void* serverSocket, MessageToServer* recv) {
        
        MessageFromServer       reply;
        
        if(FindInVectorLogin(recv->login) != -1 || FindInVectorNumber(recv->number) != -1) {
                reply.status = 0;
                SendMessage(&reply, serverSocket);
        }
        else {
                vector = AddInVector(recv->login, recv->number);
                reply.status = 1;
                SendMessage(&reply, serverSocket);
        }
        return;
}

void LogIn(void* serverSocket, MessageToServer* recv) {
        
        MessageFromServer       reply;
        int exist = FindInVectorLogin(recv->login);
        
        if(exist != -1) {
                strcpy(reply.number, vector->users[exist].number);
                reply.status = 1;
                SendMessage(&reply, serverSocket);
        }
        else {
                reply.status = 0;
                SendMessage(&reply, serverSocket);
        }
        return;
}

void AddMoney(void* serverSocket, MessageToServer* recv) {
        
        MessageFromServer       reply;
        
        int number = FindInVectorLogin(recv->login);
        
        if(number == -1) {
                reply.status = 0;
                SendMessage(&reply, serverSocket);
        }
        else {
                vector->users[number].value += recv->value;
                reply.status = 1;
                SendMessage(&reply, serverSocket);
        }
        return;
}

void AddMoneyToAnother(void* serverSocket, MessageToServer* recv, int flag) {
        
        MessageFromServer       reply;
        int                     numberThis;
        int                     numberAim;
        
        if(flag == LOGIN) {
                numberThis = FindInVectorLogin(recv->login);
                numberAim = FindInVectorLogin(recv->aimLogin); 
        }
        else {
                numberThis = FindInVectorNumber(recv->number);
                numberAim = FindInVectorNumber(recv->aimNumb); 
        }
        
        if(numberThis == -1) {
                reply.status = 0;
                SendMessage(&reply, serverSocket);
                return;
        }
        
        if(numberAim == -1 || vector->users[numberThis].value < recv->value) {
                reply.status = 0;
                SendMessage(&reply, serverSocket);
        }
        else {
                vector->users[numberThis].value -= recv->value;
                vector->users[numberAim].value += recv->value;
                reply.status = 1;
                SendMessage(&reply, serverSocket);
        }
        
        return;
}

void TakeMoney(void* serverSocket, MessageToServer* recv) {
        
        MessageFromServer       reply;
        int                     numberThis;
        
        numberThis = FindInVectorLogin(recv->login);
        
        if(numberThis == -1) {
                reply.status = 0;
                SendMessage(&reply, serverSocket);
                return;
        }
        
        if(vector->users[numberThis].value < recv->value) {
                reply.status = 0;
                SendMessage(&reply, serverSocket);
        }
        else {
                vector->users[numberThis].value -= recv->value;
                reply.status = 1;
                SendMessage(&reply, serverSocket);
        }
        return;
}

void GetValue(void* serverSocket, MessageToServer* recv) {
        
        MessageFromServer       reply;
        int                     numberThis;
        
        numberThis = FindInVectorLogin(recv->login);
        
        if(numberThis != -1) {
                reply.value = vector->users[numberThis].value;
                reply.status = 1;
                SendMessage(&reply, serverSocket);
        }
        else {
                reply.status = 0;
                SendMessage(&reply, serverSocket);
        }
        
        return;
}

void LogOut(void* serverSocket, MessageToServer* recv) {
        MessageFromServer reply;
        
        reply.status = 1;
        SendMessage(&reply, serverSocket);
        
        return;
}

void CreateNewAccount() {
        
        char            login[128];
        char            number[20];
        char            value[16];
        double          money;
        int             error;
                
        system("clear");
        printf("Enter new login.\n");
        while(1) {
                printf("> ");
                scanf("%s", login);
                if(FindInVectorLogin(login) == -1) {
                        break;
                }
                else {
                        printf("We already have that account.\n");
                }
        }
        printf("Enter new number of card. Example: 1111-2222-3333-4444\n");
        error = 1;
        while(error) {
                printf("> ");
                scanf("%s", number);
                error = 0;
                if(number[4] == '-' && number[9] == '-' && number[14] == '-' && number[19] == '\0') {
                        for(int i = 0; i < 4; ++i){
                                if(number[i] < '0' || number[i] > '9') {
                                        error = 1;
                                }
                        }
                        for(int i = 5; i < 9; ++i){
                                if(number[i] < '0' || number[i] > '9') {
                                        error = 1;
                                }
                        }
                        for(int i = 10; i < 14; ++i){
                                if(number[i] < '0' || number[i] > '9') {
                                        error = 1;
                                }
                        }
                        for(int i = 15; i < 19; ++i){
                                if(number[i] < '0' || number[i] > '9') {
                                        error = 1;
                                }
                        }
                }
                if(error) {
                        printf("Incorrect input. Try again.\n");
                }
                else {
                        if(FindInVectorNumber(number) > -1) {
                                error = 1;
                                printf("We already have that card.\n");
                        }
                }
        }
        printf("Enter the base money for account.\n");
        error = 1;
        while(error) {
                printf("> ");
                scanf("%s", value);
                error = 0;
                for(int i = 0; i < 16; ++i) {
                        if((value[i] < '0' || value[i] > '9') && value[i] != '\0' && value[i] != '.') {
                                error = 1;
                                printf("Incorrect input. Try again.\n");
                                break;
                        }
                        if(value[i] == '\0') {
                                break;
                        }
                }
        }
        money = atof(value);
        AddInVector(login, number);
        vector->users[FindInVectorLogin(login)].value = money;
        return;
}

TVector* RemoveFromVector(int index) {
        pthread_mutex_lock(&(vector->mutex));
        if(vector->current == 0) {
                printf("Vector if empty! Press enter...\n");
                getchar();
                getchar();
        }
        else {
                if(index < 0 || index >= vector->current) {
                        printf("Out of borders! Press enter...\n");
                        getchar();
                        getchar();
                        pthread_mutex_unlock(&(vector->mutex));
                        return vector;
                }
                TUser tmp;
                
                tmp.key = vector->users[vector->current - 1].key;
                strcpy(tmp.login, vector->users[vector->current - 1].login);
                strcpy(tmp.number, vector->users[vector->current - 1].number);
                tmp.value = vector->users[vector->current - 1].value;
                
                vector->users[vector->current - 1].key = vector->users[index].key;
                strcpy(vector->users[vector->current - 1].login, vector->users[index].login);
                strcpy(vector->users[vector->current - 1].number, vector->users[index].number);
                vector->users[vector->current - 1].value = vector->users[index].value;
                
                vector->users[index].key = tmp.key;
                strcpy(vector->users[index].login, tmp.login);
                strcpy(vector->users[index].number, tmp.number);
                vector->users[index].value = tmp.value;
                
                --vector->current;
                if(vector->current == 0) {
                        vector->size = 0;
                        free(vector->users);
                        vector->users = NULL;
                }
                else {
                        if(vector->current == vector->size / 2) {
                                vector->size /= 2;
                                vector->users = (TUser*) realloc(vector->users, sizeof(TUser)*vector->size);
                                if(!vector->users) {
                                        printf("ERROR: bad realloc!\n");
                                        exit(EXIT_FAILURE);
                                }
                        }
                }
        }
        pthread_mutex_unlock(&(vector->mutex));
        return vector;
}

void PrintClients(int i) {
        if(i) {
                system("clear");
        }
        printf("============================================\n");
        printf("|              List of clients             |\n");
        printf("============================================\n");
        for(int j = 0; j < vector->current; ++j) {
                printf("%d) Login - %s. Card - %s. Money - %lf\n", j + 1, vector->users[j].login, vector->users[j].number, vector->users[j].value);
        }
        printf("============================================\n");
        if(i) {
                printf("Press enter...");
                getchar();
                getchar();
        }
        return;
}

void RemoveClient(char* menu) {
        
        int error = 1;
        int number = 0;
        
        system("clear");
        printf("Which client do you want to remove\n");
        PrintClients(0);
        printf("Enter the number of client.\n");
        while(error) {
                printf("> ");
                scanf("%s", menu);
                error = 0;
                for(int i = 0; i < vector->current; ++i) {
                        if((menu[i] < '0' || menu[i] > '9') && menu[i] != '\0') {
                                printf("Incorrect input.\n");
                                error = 1;
                                break;
                        }
                        if(menu[i] == '\0') {
                                break;
                        }
                }
        }
        number = atoi(menu);
        vector = RemoveFromVector(number - 1);
        return;
}

void CheckAccount(void* serverSocket, MessageToServer* recv) {
        
        MessageFromServer               reply;
        
        int check = FindInVectorLogin(recv->login);
        
        if(check == -1) {
                reply.status = 0;
                SendMessage(&reply, serverSocket);
        }
        else {
                reply.status = 1;
                SendMessage(&reply, serverSocket);
        }
        return;
}

void* Controller(void* param) {
        
        char            menu[16];
        
        while(1) {
                system("clear");
                puts("====================");
                printf("Adress: %s\n", adress);
                puts("====================");
                if(CONTINUE == 1) {
                        puts("SERVER - STARTED");
                }
                else {
                        puts("SERVER - STOPED");
                }
                puts("====================");
                puts("Server menu:");
                puts("1. Add client.");
                puts("2. Remove client.");
                puts("3. Show clients.");
                puts("4. Stop/Start server.");
                puts("5. Close server.");
                puts("====================");
                printf("> ");
                scanf("%s", menu);
                if(menu[1] == '\0') {
                        switch(menu[0]) {
                                case '1':
                                        CreateNewAccount();
                                        break;
                                case '2':
                                        RemoveClient(menu);
                                        break;
                                case '3':
                                        PrintClients(1);
                                        break;
                                case '4':
                                        if(CONTINUE == 1) {
                                                CONTINUE = 0;
                                        }
                                        else {
                                                pthread_cond_signal(&COND_VAR);
                                                CONTINUE = 1;
                                        }
                                        break;
                                case '5':
                                        GO = 0;
                                        system("clear");
                                        pthread_exit(0);
                                        break;
                                default:
                                        printf("Wrong number!\n");
                                        break;
                        }
                }
                else {
                        printf("Incorrect input. Try again. Press enter...");
                        getchar();
                        getchar();
                }
        }
}

void Checker(void* serverSocket) {
        
        MessageFromServer answer;
        
        SendMessage(&answer, serverSocket);
}

int main() {

        MessageToServer         recv;
        pthread_mutexattr_t     mutex_attr;
        
        errno = pthread_mutexattr_init(&mutex_attr);
        if( errno ) {
                fprintf(stderr, "Error: bad mutex attribute init.\n");
                exit(EXIT_FAILURE);
        }
        
        vector = (TVector*) malloc(sizeof(TVector));
        vector->users = NULL;
        vector->size = 0;
        vector->current = 0;
        
        errno = pthread_mutex_init(&(vector->mutex), &mutex_attr);
        if( errno ) {
                fprintf(stderr, "Error: mutex init failed.\n");
                exit(EXIT_FAILURE);
        }
        
        GetPort();
        
        void*                   context = zmq_ctx_new();
        if(!context) {
                fprintf(stderr, "Error bad init context.\n");
                exit(EXIT_FAILURE);
        }
        
        void*                   serverSocket = zmq_socket(context, ZMQ_REP);
        if(!serverSocket) {
                fprintf(stderr, "Error bad create new socket.\n");
                exit(EXIT_FAILURE);
        }
        
        
        pthread_t               pthread;
        pthread_attr_t          attr;
        
        errno = pthread_condattr_init(&ATTR_COND_VAR);
        if( errno ) {
                fprintf(stderr, "Error: bad condition variable attribute init.\n");
                exit(EXIT_FAILURE);
        }
        
        errno = pthread_cond_init(&COND_VAR, &ATTR_COND_VAR);
        if( errno ) {
                fprintf(stderr, "Error: bad condition variable init.\n");
                exit(EXIT_FAILURE);
        }
        
        errno = pthread_mutexattr_init(&ATTR_CV_MUTEX);
        if( errno ) {
                fprintf(stderr, "Error: bad mutex attribute init.\n");
                exit(EXIT_FAILURE);
        }
        
        errno = pthread_mutex_init(&CV_MUTEX, &ATTR_CV_MUTEX);
        if( errno ) {
                fprintf(stderr, "Error: mutex init failed.\n");
                exit(EXIT_FAILURE);
        }
        
        errno = pthread_attr_init(&attr);
        if( errno ) {
                fprintf(stderr, "Error: bad thread attribute init.\n");
                exit(EXIT_FAILURE);
        }
        
        errno = pthread_create(&pthread, &attr, Controller, NULL);
        if( errno == EAGAIN ) {
                fprintf(stderr, "Error: a system limit is exceeded.\n");
                exit(EXIT_FAILURE);
        }
        else if ( errno == EINVAL ) {
                fprintf(stderr, "Error: the value of attr_t is invalid.\n");
                exit(EXIT_FAILURE);
        }
        else if ( errno == EPERM ) {
                fprintf(stderr, "Error: the caller does not have appropriate permission to set the required scheduling parameters or scheduling policy.\n");
                exit(EXIT_FAILURE);
        }
        
        zmq_bind(serverSocket, adress);
        
        while(GO) {
                if(CONTINUE) {
                        RecvMessage(serverSocket, &recv);
                        switch(recv.act) {
                                case 1:
                                        //Регистрация
                                        Registrate(serverSocket, &recv);
                                        break;
                                case 2:
                                        //Авторизация
                                        LogIn(serverSocket, &recv);
                                        break;
                                case 3:
                                        //Пополнить себе счёт
                                        AddMoney(serverSocket, &recv);
                                        break;
                                case 4:
                                        //Пополнить чужой счёт (карта)
                                        AddMoneyToAnother(serverSocket, &recv, LOGIN);
                                        break;
                                case 5:
                                        //Пополнить чужой счёт (логин)
                                        AddMoneyToAnother(serverSocket, &recv, CARD);
                                        break;
                                case 6:
                                        //Снять денег
                                        TakeMoney(serverSocket, &recv);
                                        break;
                                case 7:
                                        //Узнать счёт
                                        GetValue(serverSocket, &recv);
                                        break;
                                case 8:
                                        //Выйти из логина
                                        LogOut(serverSocket, &recv);
                                        break;
                                case 9:
                                        CheckAccount(serverSocket, &recv);
                                        break;
                                case 100:
                                        Checker(serverSocket);
                        }
                }
                else {
                        pthread_cond_wait(&COND_VAR, &CV_MUTEX);
                }
        }
        printf("Closing server!\n");
        zmq_close(serverSocket);
        zmq_ctx_destroy(context);
        return 0;
}
