#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main() {
    int number = -1, n, answer;
    int filePipesPrev[2], filePipesNext[2];
    pid_t forkResult = 0, current;
    do {
        printf("Enter the number: ");
        scanf("%d", &number);
        if(number <= 0) {
            puts("Please, enter the number > 0");
        }   
    } while(number <= 0);
    puts("===========\nCalculating\n===========");
    for(n = number; n > 0; n--) {
        if(number % n == 0 && (number != n && n != 1)) {
            answer = 0;
            write(filePipesPrev[1],&answer,sizeof(int));
            exit(0);
        }
        filePipesPrev[0] = filePipesNext[0];
        filePipesPrev[1] = filePipesNext[1];
        pipe(filePipesNext);
        current = forkResult;
        forkResult = fork();
        if(forkResult == 0) {
            if(number == 1) {
                puts("Answer - True!");
                exit(0);
            }
            if(read(filePipesNext[0],&answer,sizeof(int)) != 0) {
                if(answer == 1) {
                    puts("Answer - True!");
                    exit(0);
                }
                else {
                    puts("Answer - False!");
                    exit(0);
                }
                exit(0);
            }
            else {
                puts("Error: can't read answer from pipe!");
                exit(1);
            }
        }
        if(current == forkResult) {
            if(read(filePipesNext[0],&answer,sizeof(int)) != 0) {
                write(filePipesPrev[1],&answer,sizeof(int));
                exit(0);
            }
            else {
                puts("Error: can't read answer from pipe!");
                exit(1);
            }
        }
    }
    answer = 1;
    write(filePipesPrev[1],&answer,sizeof(int));
    exit(0);
}
