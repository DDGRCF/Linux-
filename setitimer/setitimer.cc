#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>


void sys_error(const char* msg) {
  perror(msg);
  exit(1);
}

unsigned int my_alarm(unsigned int sec) {
  struct itimerval it, oldit;
  it.it_value.tv_sec = sec;
  it.it_value.tv_usec = 0;
  it.it_interval.tv_sec = 2;
  it.it_interval.tv_usec = 0;
  int ret = setitimer(ITIMER_REAL, &it, &oldit);
  if (ret == -1) {
    perror("setitimer");
    exit(1);
  }
  return oldit.it_value.tv_sec;
}

int main() {
  unsigned int i = my_alarm(2);
  for ( ; ; ) {
    printf("%d\n", i);
    sleep(2);
  }
  return 0;
}