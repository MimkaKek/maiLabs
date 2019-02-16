#include <sys/types.h> 
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {

    int number, n, answer;
    int filePipesPrev[2], filePipesNext[2];
    pid_t forkResult;
    
    do {
        printf("Enter the number: ");
        scanf("%d", &number);
        if(number <= 0) {
            puts("Please, enter the number > 0");
        }   
    } while(number <= 0);
    
    puts("===========\nCalculating\n===========");
    //printf("Root Process - %d working!\n", getpid());
    //puts("Creating new node!");
    if(number == 1) {
            puts("Answer - True!");
            exit(0);
    }
    pipe(filePipesNext);
    forkResult = fork();

    if(forkResult == 0) {
        for(n = 1; n < number; n++) {
          //printf("%d. Process - %d working!\n", n, getpid());
          filePipesPrev[0] = filePipesNext[0];
          filePipesPrev[1] = filePipesNext[1];
          if(number == 1) {
            answer = 1;
            write(filePipesPrev[1],&answer,sizeof(int));
            exit(0);
          }
          if(number % n == 0 && (number != n && n != 1)) {
            //printf("%d. Answer finded! Proccess %d stop working!\n", n, getpid());
            answer = 0;
            write(filePipesPrev[1],&answer,sizeof(int));
            exit(0);
          }
          pipe(filePipesNext);
          //printf("Pipes: before %d and %d, after %d and %d\n", filePipesPrev[0], filePipesPrev[1], filePipesNext[0],filePipesNext[1]);
          forkResult = fork();
          if(forkResult != 0) {
            //printf("Proccess %d wait for reading!\n", getpid());
            wait(NULL);
            if(read(filePipesNext[0],&answer,sizeof(int)) != 0) {
              //printf("Get answer - %d ! Process - %d stop working!\n", answer, getpid());
              //printf("Proccess %d start write!\n", getpid());
              write(filePipesPrev[1],&answer,sizeof(int));
              exit(0);
            }
            else {
              puts("Error: can't read answer from pipe!");
              answer = -2;
              write(filePipesPrev[1],&answer,sizeof(int));
              exit(1);
            }
          }
          else if(forkResult == -1) {
            perror("fork");
            answer = -1;
            write(filePipesPrev[1],&answer,sizeof(int));
            exit(1);
          }
        }
    }
    else if(forkResult != -1) {
        wait(NULL);
        if(read(filePipesNext[0],&answer,sizeof(int)) != 0) {
            if(answer == 1) {
                puts("Answer - True!");
                exit(0);
            }
            else if (answer == 0) {
                puts("Answer - False!");
                exit(0);
            }
            else if (answer < 0) {
              exit(1);
            }
            exit(0);
        }
        else {
            puts("Error: can't read answer from pipe!");
            exit(1);
        }
    }
    else {
      perror("fork");
      exit(1);
    }
    answer = 1;
    write(filePipesNext[1],&answer,sizeof(int));
    //printf("Answer finded! Proccess %d stop working!\n", getpid());
    exit(0);
}
