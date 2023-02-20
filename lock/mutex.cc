#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t mutex;
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER

void sys_error(const char* msg, int ret) {
  fprintf(stderr, "%s, errno=%d: %s\n", msg, ret, strerror(ret));
  exit(1);
}

void* tfn(void* arg) {
  for (;;) {
    sleep(rand() % 5);
    int ret = pthread_mutex_lock(&mutex);
    if (ret != 0) {
      sys_error("mutex", ret);
    }
    printf("hello");
    printf("world!\n");
    printf("I'am thread: %d, tid=%lu\n", getpid(), pthread_self());
    ret = pthread_mutex_unlock(&mutex);
    if (ret != 0) {
      sys_error("mutex", ret);
    }
  }
}

int main() {
  srand(time(nullptr));
  pthread_t tid;
  pthread_attr_t attr;
  const int kNumThread = 5;
  int ret;

  ret = pthread_mutex_init(&mutex, nullptr);
  if (ret != 0) {
    sys_error("mutex create", ret);
  }
  ret = pthread_attr_init(&attr);
  if (ret != 0) {
    sys_error("pthread attr init", ret);
  }
  ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  if (ret != 0) {
    sys_error("pthread setattr", ret);
  }

  int i;
  for (int i = 0; i < kNumThread; i++) {
    ret = pthread_create(&tid, &attr, tfn, nullptr);
    if (ret != 0) {
      sys_error("pthread create", ret);
    }
  }

  ret = pthread_attr_destroy(&attr);
  if (ret != 0) {
    sys_error("pthread destory", ret);
  }

  for (;;) {
    sleep(rand() % 5);
    ret = pthread_mutex_lock(&mutex);
    if (ret != 0) {
      sys_error("mutex", ret);
    } 
    printf("HELLO");
    printf("WORLD!\n");
    ret = pthread_mutex_unlock(&mutex);
    if (ret != 0) {
      sys_error("mutex", ret);
    } 
  }

  ret = pthread_mutex_destroy(&mutex);
  if (ret != 0) {
    sys_error("mutex", ret);
  }
  return 0;
}