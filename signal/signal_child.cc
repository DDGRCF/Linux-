#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void sys_error(const char* msg) {
  perror(msg);
  exit(1);
}

void catch_child(int num) {
  pid_t wpid;
  int status;
  while ((wpid = waitpid(0, &status, 0)) != -1) {
    if (WIFEXITED(status)) { // 判断信号
      printf("catch child: %d, ret=%d\n", wpid, WEXITSTATUS(status)); // 读取信号
    }    
  }
}

int main() {
  const int kNumPro = 5;
  pid_t pid;
  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGCHLD);
  sigprocmask(SIG_BLOCK, &sigset, nullptr); // 使用屏蔽字，防止先启动

  int i;
  for (i == 0; i < kNumPro; i++) {
    pid = fork();
    if (pid == 0) {
      break;
    }
  }

  if (i == kNumPro) {
    printf("I'am parent: %d, before\n", getpid());
    struct sigaction act;
    act.sa_handler = catch_child;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, nullptr);
    printf("I'am parent: %d, after\n", getpid());
    sigprocmask(SIG_UNBLOCK, &sigset, nullptr); // 解除屏蔽字
    while (true);
  } else {
    printf("I'am child: %d\n", getpid());
    return i;
  }

  return 0;
}