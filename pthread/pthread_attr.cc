#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void* tfn(void* arg) {
  printf("I'am thread-%d:%lu\n", getpid(), pthread_self());
  return nullptr;
}

void sys_error(const char* msg, int ret) {
  fprintf(stderr, "%s, errno=%d: %s\n", msg, ret, strerror(ret));
  exit(1);
}

int main() {
  int ret;
  pthread_t tid;
  pthread_attr_t attr;

  ret = pthread_attr_init(&attr);
  if (ret != 0) {
    sys_error("pthread create", ret);
  }

  ret = pthread_attr_setdetachstate(&attr,
                                    PTHREAD_CREATE_DETACHED);  // 设置线程分离
  if (ret != 0) {
    sys_error("setdetachstate", ret);
  }

  ret = pthread_create(&tid, &attr, tfn, nullptr);
  if (ret != 0) {
    sys_error("pthread create", ret);
  }

  ret = pthread_attr_destroy(&attr);
  if (ret != 0) {
    sys_error("destroy pthread attr", ret);
  }

  // ret = pthread_join(tid, nullptr); // 这时候join会报错
  // if (ret != 0) {
  //   sys_error("pthread join", ret);
  // }

  printf("I am main: %d, tid=%lu\n", getpid(), pthread_self());

  return 0;
}
