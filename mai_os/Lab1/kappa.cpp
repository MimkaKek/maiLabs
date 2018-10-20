#include <sys/types.h>
/*
#include <sys/wait.h>
#include <iostream>
#include <сstdlib>
#include <unistd.h>
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
  int data_processed;
 int file_pipes[2];
 const char some_data[] = "1213";
 int buffer;
 pid_t fork_result;
 if (pipe(file_pipes) == 0) {
  fork_result = fork();
  if (fork_result == -1) {
   fprintf(stderr, "Fork failure");
   exit(EXIT_FAILURE);
  }
  if (fork_result == 0) {
   data_processed = read(file_pipes[0], &buffer, sizeof(int)*2);
   printf("Read %d bytes: %d\n", data_processed, buffer);
   exit(EXIT_SUCCESS);
  }
  else {
   int a, b;
   scanf("%d %d", &a, &b);
   int c = a + b;
   data_processed = write(file_pipes[1], &c, sizeof(int));
   printf("Wrote %d bytes\n", data_processed);
  }
 }
 exit(EXIT_SUCCESS);
}
