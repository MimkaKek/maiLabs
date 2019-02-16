#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include "zmq.h"

/* acts:
 * 1 - регистрация
 * 2 - авторизация
 * 3 - пополнить счёт
 * 4 - пополнить чужой счёт (логин)
 * 5 - пополнить чужой счёт (номер карты)
 * 6 - снять деняк
 * 7 - узнать свой счёт
 * 8 - выйти из логина
 */

char adress[] = {'t','c','p',':','/','/','l','o','c','a','l','h','o','s','t',':','0','0','0','0', '\0'};

int CONNECTED = 0;

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

typedef struct AT {
        void*                   context;
        void*                   socket;
} AttrThread;

void SendMessage(MessageToServer* msg, void* senderSocket) {
        zmq_msg_t zmqMessage;
        zmq_msg_init_size(&zmqMessage, sizeof(MessageToServer));
        memcpy(zmq_msg_data(&zmqMessage), msg, sizeof(MessageToServer));
        zmq_sendmsg(senderSocket, &zmqMessage, 0);
        zmq_msg_close(&zmqMessage);
        return;
}

int RecvMessage(void* senderSocket, MessageFromServer* recv) {
        
        int rc;
        
        zmq_msg_t reply;
        zmq_msg_init(&reply);
        if(CONNECTED) {
                rc = zmq_msg_recv(&reply, senderSocket, 0);
        }
        else {
                rc = zmq_msg_recv(&reply, senderSocket, ZMQ_DONTWAIT);
        }
        if(rc != -1) {
                printf("Message recieved.\n");
        }
        else {
                printf("Message not recieved.\n");
                zmq_msg_close(&reply);
                return 0;
        }
        MessageFromServer* tmp1 = (MessageFromServer*) zmq_msg_data(&reply);
        
        recv->status = tmp1->status;
        recv->value = tmp1->value;
        memcpy(recv->number, tmp1->number, sizeof(char)*20);
        
        zmq_msg_close(&reply);
        return 1;
}

int WorkWithMsg(MessageToServer* msg, void* senderSocket, MessageFromServer* recv) {
        SendMessage(msg, senderSocket);
        if(RecvMessage(senderSocket, recv)) {
                return 1;
        }
        else {
                return 0;
        }
}

int Register(MessageToServer* msg, void* senderSocket) {
        
        MessageFromServer       recv;
        char                    menu[10];
        int                     error = 1;
        
        system("clear");
        printf("Want Exit? ('y' or 'n')\n");
        while(error) {
                printf("> ");
                scanf("%s", menu);
                if(menu[1] == '\0') {
                        switch(menu[0]) {
                                case 'y':
                                        return 1;
                                case 'n':
                                        error = 0;
                                        break;
                                default:
                                        printf("Incorrect input. Try again. Press enter...");
                                        getchar();
                                        getchar();
                        }
                }
                else {
                        printf("Incorrect input. Try again. Press enter...");
                        getchar();
                        getchar();
                }
        }
        while(1) {
                system("clear");
                printf("Creating new account.\n");
                printf("Enter your login: ");
                scanf("%s", msg->login);
                printf("Enter your number of card.\nExample: 1111-2222-3333-4444\n");
                error = 1;
                while(error) {
                        printf("> ");
                        scanf("%s", msg->number);
                        error = 0;
                        if(msg->number[4] == '-' && msg->number[9] == '-' && msg->number[14] == '-' && msg->number[19] == '\0') {
                                for(int i = 0; i < 4; ++i){
                                        if(msg->number[i] < '0' || msg->number[i] > '9') {
                                                error = 1;
                                        }
                                }
                                for(int i = 5; i < 9; ++i){
                                        if(msg->number[i] < '0' || msg->number[i] > '9') {
                                                error = 1;
                                        }
                                }
                                for(int i = 10; i < 14; ++i){
                                        if(msg->number[i] < '0' || msg->number[i] > '9') {
                                                error = 1;
                                        }
                                }
                                for(int i = 15; i < 19; ++i){
                                        if(msg->number[i] < '0' || msg->number[i] > '9') {
                                                error = 1;
                                        }
                                }
                        }
                        else {
                                error = 1;
                        }
                        if(error) {
                                printf("Incorrect input. Try again.\n");
                        }
                }
                msg->act = 1;
                if(WorkWithMsg(msg, senderSocket, &recv)) {
                        if(recv.status) {
                                printf("Succesfully!\nPlease login\n");
                                printf("Press Enter...\n");
                                getchar();
                                getchar();
                                break;
                        }
                        else {
                                printf("Registration failed. We already have the same login or number of card.\nPress enter...");
                                getchar();
                                getchar();
                        }
                }
                else {
                        return 0;
                }
        }
        return 1;
}

int LogIn(MessageToServer* msg, void* senderSocket) {
        
        MessageFromServer       recv;
        char                    menu[10];
        int                     attempt = 0;
        int                     error = 1;
        
        system("clear");
        printf("Want Exit? ('y' or 'n')\n");
        while(error) {
                printf("> ");
                scanf("%s", menu);
                if(menu[1] == '\0') {
                        switch(menu[0]) {
                                case 'y':
                                        return -1;
                                case 'n':
                                        error = 0;
                                        break;
                                default:
                                        printf("Incorrect input. Try again. Press enter...");
                                        getchar();
                                        getchar();
                        }
                }
                else {
                        printf("Incorrect input. Try again. Press enter...");
                        getchar();
                        getchar();
                }
        }
        while(1) {
                system("clear");
                printf("Enter your login: ");
                scanf("%s", msg->login);
                printf("Wait please... ");
                msg->act = 2;
                if(WorkWithMsg(msg, senderSocket, &recv)) {
                        if(recv.status) {
                                strcpy(msg->number, recv.number);
                                printf("Success!\nPress enter...");
                                getchar();
                                getchar();
                                break;
                        }
                        else if(attempt < 3){
                                ++attempt;
                                printf("failed.\nIncorrect login or password. Attempts left - %d.\nPress enter...", 3 - attempt);
                                getchar();
                                getchar();
                        }
                        else {
                                printf("Return to menu! Press enter...");
                                getchar();
                                getchar();
                                return -1;
                        }
                }
                else {
                        return 0;
                }
        }
        return 1;
}

void LoginToServer(MessageToServer* msg, void* senderSocket, void* context, char* menu, int* login) {
        
        int             error = 1;
        int             check;
        
        while(error) {
                system("clear");
                printf("Welcome!\n");
                printf("1) Log in.\n");
                printf("2) Register.\n");
                printf("3) Exit\n");
                printf("> ");
                scanf("%s", menu);
                if((menu[0] >= '1' && menu[0] <= '3') && menu[1] == '\0') {
                        switch(menu[0]) {
                                case '1':
                                        check = LogIn(msg, senderSocket);
                                        if(!check) {
                                                return;
                                        }
                                        else if(check == 1) {
                                                error = 0;
                                                break;
                                        }
                                        else {
                                                break;
                                        }
                                case '2':
                                        check = Register(msg, senderSocket) 
                                        if(!check){
                                                return;
                                        }
                                        else if(check == 1) {
                                                error = 0;
                                                break;
                                        }
                                        else {
                                                break;
                                        }
                                        check = LogIn(msg, senderSocket);
                                        if(!check) {
                                                return;
                                        }
                                        else if(check == 1) {
                                                error = 0;
                                                break;
                                        }
                                        else {
                                                break;
                                        }
                                case '3':
                                        zmq_close(senderSocket);
                                        zmq_ctx_destroy(context);
                                        exit(0);
                        }
                }
                else {
                        printf("Incorrect input. Try again!\n");
                        getchar();
                        getchar();
                }
        }
        *login = 1;
        return;
}

void GetPort(char* menu) {
        system("clear");
        printf("Enter the port. You need 4 numbers.\n");
        
        int             error = 1;
        int             counter = 0;
        
        while(error) {
                printf("> ");
                scanf("%s", menu);
                counter = 0;
                for(int i = 0; i < 4; ++i) {
                        if(menu[i] >= '0' && menu[i] <= '9') {
                                adress[16 + i] = menu[i];
                                ++counter;
                        }
                        else {
                                break;
                        }
                }
                if(counter == 4 && menu[4] == '\0') {
                        error = 0;
                }
                else {
                        printf("Wrong input. Try again.\n");
                }
        }
}

int SendMoneyToMe(MessageToServer* msg, void* senderSocket, char* menu) {
        
        int                     error = 1;
        MessageFromServer       recv;
        
        msg->act = 3;
        
        while(1) {
                system("clear");
                printf("How many money: ");
                scanf("%s", menu);
                error = 0;
                for(int i = 0; i < 16; ++i) {
                        if((menu[i] < '0' || menu[i] > '9') && menu[i] != '\0' && menu[i] != '.') {
                                error = 1;
                                printf("Incorrect input. Try again. Press enter...");
                                getchar();
                                getchar();
                                break;
                        }
                        if(menu[i] == '\0') {
                                break;
                        }
                }
                if(!error) {
                        msg->value = atof(menu);
                        if(WorkWithMsg(msg, senderSocket, &recv)) {
                                if(recv.status) {
                                        system("clear");
                                        printf("Success! Press enter...");
                                        getchar();
                                        getchar();
                                        break;
                                }
                                else {
                                        system("clear");
                                        printf("Something go wrong. Press enter...");
                                        getchar();
                                        getchar();
                                        break;
                                }
                        }
                        else {
                                return 0;
                        }
                }
        }
        return 1;
}

int SendMoneyToSomeone(MessageToServer* msg, void* senderSocket, char* menu) {
        
        int                     error = 1;
        MessageFromServer       recv;
        
        system("clear");
        printf("You will enter login ('l') or number of card ('n')?\n");
        
        while(1) {
                printf("> ");
                scanf("%s", menu);
                if((menu[0] != 'l' && menu[0] != 'n') || menu[1] != '\0') {
                        printf("Incorrect input. Try again.\n");
                }
                else if(menu[0] == 'l') {
                        msg->act = 4;
                        printf("Enter the login\n");
                        printf("> ");
                        scanf("%s", msg->aimLogin);
                        break;
                }
                else {
                        msg->act = 5;
                        printf("Enter number of the card.\n");
                        error = 1;
                        while(error) {
                                printf("> ");
                                scanf("%s", msg->aimNumb);
                                error = 0;
                                for(int i = 0; i < 16; ++i) {
                                        if((menu[i] < '0' || menu[i] > '9') && menu[i] != '\0') {
                                                error = 1;
                                                printf("Incorrect input. Try again. Press enter...");
                                                getchar();
                                                getchar();
                                                break;
                                        }
                                        if(menu[i] == '\0') {
                                                break;
                                        }
                                }
                        }
                        break;
                }
        }
        printf("How many money?\n");
        while(1) {
                printf("> ");
                scanf("%s", menu);
                error = 0;
                for(int i = 0; i < 16; ++i) {
                        if((menu[i] < '0' || menu[i] > '9') && menu[i] != '\0' && menu[i] != '.') {
                                error = 1;
                                printf("Incorrect input. Try again.\n");
                                break;
                        }
                        if(menu[i] == '\0') {
                                break;
                        }
                }
                if(!error) {
                        msg->value = atof(menu);
                        if(WorkWithMsg(msg, senderSocket, &recv)) {
                                if(recv.status) {
                                        system("clear");
                                        printf("Success! Press enter...");
                                        getchar();
                                        getchar();
                                        break;
                                }
                                else {
                                        system("clear");
                                        printf("Something go wrong. Press enter...");
                                        getchar();
                                        getchar();
                                        break;
                                }
                        }
                        else {
                                return 0;
                        }
                        
                }
        }
        return 1;
}

int GetMoney(MessageToServer* msg, void* senderSocket, char* menu) {
        
        MessageFromServer       recv;
        int                     error;
        
        system("clear");
        printf("How many money?\n");
        
        msg->act = 6;
        
        while(1) {
                printf("> ");
                scanf("%s", menu);
                error = 0;
                for(int i = 0; i < 16; ++i) {
                        if((menu[i] < '0' || menu[i] > '9') && menu[i] != '\0' && menu[i] != '.') {
                                error = 1;
                                printf("Incorrect input. Try again.\n");
                                break;
                        }
                        if(menu[i] == '\0') {
                                break;
                        }
                }
                if(!error) {
                        msg->value = atof(menu);
                        if(WorkWithMsg(msg, senderSocket, &recv)) {
                                if(recv.status) {
                                        system("clear");
                                        printf("Success! Press enter...");
                                        getchar();
                                        getchar();
                                        break;
                                }
                                else {
                                        system("clear");
                                        printf("Something go wrong. Press enter...");
                                        getchar();
                                        getchar();
                                        break;
                                }
                        }
                        else {
                                return 0;
                        }
                }
        }
        return 1;
}

int CheckBalance(MessageToServer* msg, void* senderSocket) {
        
        MessageFromServer       recv;
        
        msg->act = 7;
        system("clear");
        
        if(WorkWithMsg(msg, senderSocket, &recv)) {
                if(recv.status) {
                        printf("Balance: %lf\n", recv.value);
                        printf("Press enter...");
                        getchar();
                        getchar();
                }
                else {
                        printf("Something go wrong. Press enter...");
                        getchar();
                        getchar();
                }
        }
        else {
                return 0;
        }
        return 1;
}

int CheckAccount(MessageToServer* msg, void* senderSocket) {
        
        MessageFromServer recv;
        
        msg->act = 9;
        if(WorkWithMsg(msg, senderSocket, &recv)) {
                if(recv.status) {
                        system("clear");
                        printf("Login: %s\n", msg->login);
                        printf("Number of card: %s\n", msg->number);
                        printf("Press enter...");
                        getchar();
                        getchar();
                }
                else {
                        system("clear");
                        printf("Your account was deleted. Press enter...");
                        getchar();
                        getchar();
                        return -1;
                }
        }
        else {
                return 0;
        }
        
        
        return 1;
}

int LogOut(MessageToServer* msg, void* senderSocket, char* menu, int* login) {
        
        MessageFromServer recv;
        
        msg->act = 8;
        
        system("clear");
        printf("Are you sure? ('y' or 'n')\n");
        while(1) {
                printf("> ");
                scanf("%s", menu);
                if(menu[1] == '\0' && (menu[0] == 'y' || menu[0] == 'n')) {
                        if(menu[0] == 'y') {
                                *login = 0;
                                if(WorkWithMsg(msg, senderSocket, &recv)) {
                                        printf("Success! Press enter...");
                                        getchar();
                                        getchar();
                                        break;
                                }
                                else {
                                        return 0;
                                }
                        }
                        else {
                                break;
                        }
                }
                else {
                        printf("Incorrect input. Try again.\n");
                }
        }
        return 1;
}

void ExitFromProgram(MessageToServer* msg, void* senderSocket, void* context, char* menu) {
        
        msg->act = 8;
        
        system("clear");
        printf("Are you sure? ('y' or 'n')\n");
        while(1) {
                printf("> ");
                scanf("%s", menu);
                if(menu[1] == '\0' && (menu[0] == 'y' || menu[0] == 'n')) {
                        if(menu[0] == 'y') {
                                zmq_close(senderSocket);
                                zmq_ctx_destroy(context);
                                exit(0);
                        }
                        else {
                                break;
                        }
                }
                else {
                        printf("Incorrect input. Try again.\n");
                }
        }
        return;
}

void* Checker(void* param) {
        
        void*                   pthreadContext = zmq_ctx_new();
        void*                   pthreadSocket = zmq_socket(pthreadContext, ZMQ_REQ);
        
        MessageToServer         msg;
        
        msg.act = 100;
        
        zmq_connect(pthreadSocket, adress);
        
        for(;;){
                SendMessage(&msg, pthreadSocket);
                sleep(1);
                zmq_msg_t reply;
                zmq_msg_init(&reply);
                int rc = zmq_msg_recv(&reply, pthreadSocket, ZMQ_DONTWAIT);
                if(rc == -1) {
                        zmq_msg_close(&reply);
                        CONNECTED = 0;
                }
                else {
                        zmq_msg_close(&reply);
                        CONNECTED = 1;
                }
        }
}

int main() {
        
        char                    menu[16];
        int                     login = 0;
        int                     timer;
        int                     check;
        
        MessageToServer         msg;
        MessageFromServer       recv;
        
        void*                   context = zmq_ctx_new();
        if(!context) {
                fprintf(stderr, "Error bad init context.\n");
                exit(EXIT_FAILURE);
        }
        
        void*                   senderSocket = zmq_socket(context, ZMQ_REQ);
        if(!senderSocket) {
                fprintf(stderr, "Error bad create new socket.\n");
                exit(EXIT_FAILURE);
        }
        
        GetPort(menu);
        printf("Your adress: %s\n", adress);
        
        pthread_t               pthread;
        pthread_attr_t          attr;
        
        errno = pthread_attr_init(&attr);
        if( errno ) {
                fprintf(stderr, "Error: bad thread attribute init.\n");
                exit(EXIT_FAILURE);
        }
        
        errno = pthread_create(&pthread, &attr, Checker, NULL);
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
        
        if(zmq_connect(senderSocket, adress)) {
                fprintf(stderr, "Error: bad zmq_connect.\n");
                exit(EXIT_FAILURE);
        }
        
        printf("Connecting...\n");
        
        while(1) {
                LoginToServer(&msg, senderSocket, context, menu, &login);
                while(login && CONNECTED) {
                        system("clear");
                        puts("Main menu:");
                        puts("1) Send money to this account;");
                        puts("2) Send money to another account;");
                        puts("3) Get money from this account;");
                        puts("4) Check balance;");
                        puts("5) Check number of card and login;");
                        puts("6) Log out;");
                        puts("7) Exit.");
                        printf("> ");
                        scanf("%s", menu);
                        if(menu[1] == '\0') {
                                switch(menu[0]) {
                                        case '1':
                                                SendMoneyToMe(&msg, senderSocket, menu);
                                                break;
                                        case '2':
                                                SendMoneyToSomeone(&msg, senderSocket, menu);
                                                break;
                                        case '3':
                                                GetMoney(&msg, senderSocket, menu);
                                                break;
                                        case '4':
                                                CheckBalance(&msg, senderSocket);
                                                break;
                                        case '5':
                                                check = CheckAccount(&msg, senderSocket);
                                                if(check == 1) {
                                                        break;
                                                }
                                                else if(check == -1) {
                                                        login = 0;
                                                        break;
                                                }
                                        case '6':
                                                LogOut(&msg, senderSocket, menu, &login);
                                                break;
                                        case '7':
                                                ExitFromProgram(&msg, senderSocket, context, menu);
                                                break;
                                        default:
                                                printf("Incorrect input. Try again. Press Enter...");
                                                getchar();
                                                getchar();
                                                break;
                                }
                        }
                        else {
                                printf("Incorrect input. Try again. Press Enter...");
                                getchar();
                                getchar();
                        }
                }
                if(!CONNECTED) {
                        timer = 120;
                        login = 0;
                        while(timer > 0 && !CONNECTED) {
                                system("clear");
                                printf("Connection have lost. Trying to reconnect!\n");
                                printf("=============================\n");
                                printf("|     Time left - %2d:%2d     |\n", timer / 60, timer % 60);
                                printf("=============================\n");
                                --timer;
                                msg.act = 100;
                                SendMessage(&msg, senderSocket);
                                sleep(1);
                                if(RecvMessage(senderSocket, &recv)) {
                                        CONNECTED = 1;
                                        printf("Connection get! Please relog!\nPress enter...");
                                        getchar();
                                        getchar();
                                }
                        }
                        if(!CONNECTED) {
                                system("clear");
                                printf("Closing client!\n");
                                return 1;
                        }
                }
        }
        return 0;
}
