#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

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
    pid_t wpid = waitpid(tmppid, nullptr, 0); // 非阻塞
    if (wpid == -1) {
      perror("wait pid error");
    }
    wpid = waitpid(-1, nullptr, WNOHANG);
    if (wpid == -1) {
      perror("wait pid error");
    }
    printf("I'am parent: %d, after waitpid\n", getpid());
  } else {
    int sleep_time = rand() % 10;
    sleep(sleep_time);
    printf("I'am child: %d, sleep: %ds\n", getpid(), sleep_time);
  }

  return 0;
}