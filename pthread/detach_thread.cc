#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

void* tfn(void* arg) {
  printf("I'am thread-%d:%lu\n", getpid(), pthread_self());
  return nullptr;
}

void sys_error(const char* msg, int ret=errno) {
  fprintf(stderr, "%s, errno=%d:%s\n", msg, ret, strerror(ret));
  exit(1);
}

int main() {
  int ret;
  pthread_t tid;

  ret = pthread_create(&tid, nullptr, tfn, nullptr);
  if (ret != 0) {
    sys_error("create pthread");
  }
  ret = pthread_detach(tid);
  if (ret != 0) {
    sys_error("pthread detach");
  }

  sleep(1);
  ret = pthread_join(tid, nullptr); // 由于detach后系统自动回收，再手动回收会出现错误
  printf("join, ret=%d\n", ret);
  if (ret != 0) {
    sys_error("pthread join", ret);
  }
  printf("main: pid=%d, tid=%lu\n", getpid(), pthread_self());
  return 0;
}