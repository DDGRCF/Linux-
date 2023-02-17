#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>


int main() {
  int status;
  pid_t pid, wpid;
  pid = fork();
  if (pid == -1) {
    perror("fork error");
    exit(1);
  } else if (pid == 0) {
    printf("I'am child: %d, my parent pid: %d\n", getpid(), getppid());
    sleep(10);
  } else if (pid > 0) {
    wpid = wait(&status);
    if (wpid == -1) {
      perror("wait error");
      exit(1);
    }

    if (WIFEXITED(status)) {
      printf("child exit with %d\n", WEXITSTATUS(status));
    }
    if (WIFSIGNALED(status)) {
      printf("child kill with signal %d\n", WTERMSIG(status));
    }

    if (pid == wpid) {
      printf("I'am parent: %d, my child pid: %d\n", getpid(), pid);
    } else {
      printf("I'am parent: %d, get child pid error\n", getpid());
    }
  }
  return 0;
}