#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void sys_error(const char* msg) {
  perror(msg);
  exit(1);
}

void print_set(sigset_t* set) {
  int i;
  for (i = 0; i < 32; i++) {
    if (sigismember(set, i)) {
      putchar('1');
    } else {
      putchar('0');
    }
  }
  printf("\n");
}

int main() {
  sigset_t sigset, old_sigset, ped_set;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGKILL);
  sigaddset(&sigset, SIGQUIT);
  sigaddset(&sigset, SIGINT);
  sigprocmask(SIG_BLOCK, &sigset, &old_sigset);
  int ret;
  while (true) {
    ret = sigpending(&ped_set); 
    print_set(&ped_set);
    sleep(1);
  }
  return 0;
}