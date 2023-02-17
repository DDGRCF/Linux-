#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int main() {
  pid_t pid = fork();
  if (pid == -1) {
    perror("fork error");
    exit(1);
  } else if (pid == 0) {
    printf("I'am child: %d, my parent pid: %d\n", getpid(), getppid());
  } else if (pid > 0) {
    // sleep(2);
    printf("I'am parent: %d, my child pid: %d\n", getpid(), pid);
  }
  return 0;
}