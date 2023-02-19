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

void sig_catch(int num) {
  printf("catch signal: %d\n", num);
  sleep(10);
} 

int main() {
  int ret;
  struct sigaction act, oldact;
  act.sa_handler = sig_catch;
  sigemptyset(&act.sa_mask);
  sigaddset(&act.sa_mask, SIGQUIT); // 在信号执行期间，屏蔽其他信号
  act.sa_flags = 0;
  ret = sigaction(SIGINT, &act, &oldact);
  if (ret == -1) {
    sys_error("sigaction error");
  }
  // ret = sigaction(SIGQUIT, &act, &oldact);
  // if (ret == -1) {
  //   sys_error("sigaction error");
  // }
  while (true);
  return 0;
}