#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


void sys_error(const char* msg) {
  perror(msg);
  exit(1);
}

int main() {
  pid_t pid = fork();
  if (pid > 0) {
    printf("I'am parent: %d, wait...\n", getpid());
    while(true);
  } else if (pid == 0) {
    printf("I'am child: %d, parent pid=%d\n", getpid(), getppid());
    sleep(2);
    kill(getppid(), SIGKILL);
  }
}