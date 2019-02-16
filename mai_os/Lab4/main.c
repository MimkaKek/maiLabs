#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {

    int     number, n, answer;
    int     check;
    int*    addrPrev;
    int*    addrNext;
    pid_t   forkResult;

    for(;;) {
        printf("Enter the number: ");
        check = scanf("%d", &number);
        if(check == 1 && number <= 0) {
            fprintf(stderr, "Number can't be less or equal zero.\n");
        }
        else if(check == 0) {
            fprintf(stderr, "Incorrect input.\n");
            exit(1);
        }
        else {
            break;
        }
    }
    puts("===========\nCalculating\n===========");
    addrNext = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if( addrNext == MAP_FAILED ) {
        fprintf(stderr,"Error: bad memory map.\n");
        exit(1);
    }
    forkResult = fork();
    if(forkResult == 0) {
        /* Промежуточный процесс проверяет делимость.
         */
        for(n = 1; n < (number / 2 + 1); n++) {
            addrPrev = addrNext;
            if(number == 1) {
                *addrPrev = 1;
                exit(0);
            }
            if(number % n == 0 && n != 1) {
                *addrPrev = 0;
                exit(0);
            }
            addrNext = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
            if( addrNext == MAP_FAILED ) {
                fprintf(stderr,"Error: bad memory map.\n");
                *addrPrev = -1;
                exit(1);
            }
            /* Разделение на 2 процесса - родительский ждёт дочерний. 
             * Дочерний переходит в очередной проверке на делимость.
             */
            forkResult = fork();          
            if(forkResult != 0) {
                /*Промежуточные процессы начинают ждать отклика из дочерних процессов. 
                 * Результат передают своему родителю.
                 */
                wait(NULL);
                if(*addrNext >= 0) {
                    *addrPrev = *addrNext;
                    if(munmap(addrNext, sizeof(int)) == -1) {
                        fprintf(stderr,"Error: bad memory unmap\n");
                        *addrPrev = -1;
                        exit(1);
                    }
                    exit(0);
                }
                else {
                    *addrPrev = *addrNext;
                    if(munmap(addrNext, sizeof(int)) == -1) {
                        fprintf(stderr,"Error: bad memory unmap\n");
                        *addrPrev = -1;
                        exit(1);
                    }
                    exit(1);
                }
            }
            else if(forkResult == -1) {
                /* Ошибка в работе fork.
                 */
                fprintf(stderr, "Error: bad fork result.\n");
                if(munmap(addrNext, sizeof(int)) == -1) {
                    fprintf(stderr,"Error: bad memory unmap\n");
                    *addrPrev = -1;
                    exit(1);
                }
                exit(1);
            }
        }
        
    }
    else if(forkResult != -1) {
        /* Первоначальный процесс ждёт отклика своего дочернего процесса и выводит результат.
         */
        wait(NULL);
        if(*addrNext >= 0) {
            if(*addrNext == 1) {
                puts("Answer - True!");
                if(munmap(addrNext, sizeof(int)) == -1) {
                    fprintf(stderr,"Error: bad memory unmap\n");
                    *addrPrev = -1;
                    exit(1);
                }
                exit(0);
            }
            else {
                puts("Answer - False!");
                if(munmap(addrNext, sizeof(int)) == -1) {
                    fprintf(stderr,"Error: bad memory unmap\n");
                    *addrPrev = -1;
                    exit(1);
                }
                exit(0);
            }
        }
        else {
            /* Случай одной из нескольких ошибок.
             */
            if(munmap(addrNext, sizeof(int)) == -1) {
                fprintf(stderr,"Error: bad memory unmap\n");
                *addrPrev = -1;
                exit(1);
            }
            exit(1);
        }
        
    }
    else {
        /* Ошибка в работе fork.
         */
        fprintf(stderr, "Bad fork result!\n");
        exit(1);
    }
    *addrNext = 1;
    exit(0);
}
