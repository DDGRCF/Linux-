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

void sig_handler(int num) {
  printf("receive the signal %d\n", num);
  exit(1);
}

int main() {
  int i = 0;
  signal(SIGALRM, sig_handler);
  alarm(2);
  while(true);
  return 0;
}