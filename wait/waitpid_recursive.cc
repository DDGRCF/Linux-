#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>

int main() {
  const int kNumProcess = 5;
  int i;
  pid_t pid, tmppid;
  for (i = 0; i < kNumProcess; i++) {
    pid = fork();
    if (pid == -1) {
      perror("fork error");
    }
    if (pid == 0) {
      break;
    }
    if (i == 2) {
      tmppid = pid;
    }
  }

  if (i == kNumProcess) {
    printf("I'am parent: %d, before waitpid, pid = %d\n", getpid(), tmppid);
    pid_t wpid;
    while ((wpid = waitpid(-1, nullptr, WNOHANG)) != -1) {
      if (wpid > 0) {
        printf("I'am parent: %d, get child: %d\n", getpid(), wpid);
      } else if (wpid == 0) {
        sleep(1);
      }
    }
    printf("I'am parent: %d, after waitpid\n", getpid());
  } else {
    srand(i);
    int sleep_time = rand() % 5;
    sleep(sleep_time);
    printf("I'am child: %d, sleep: %ds\n", getpid(), sleep_time);
  }

  return 0;
}