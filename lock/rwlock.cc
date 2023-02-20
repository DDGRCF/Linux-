#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>

pthread_rwlock_t rwlock;
int count = 0;

void sys_error(const char* msg, int ret) {
  fprintf(stderr, "%s, errno=%d: %s\n", msg, ret, strerror(ret));
  exit(1);
}

void* tfn(void* arg) {
  int t;
  int i = static_cast<int>(reinterpret_cast<intptr_t>(arg));
  for (;;) {
    t = count;
    sleep(1);
    pthread_rwlock_wrlock(&rwlock);
    printf("I'am writer: %d, read: %d, write: %d\n", i, t, count++);
    pthread_rwlock_unlock(&rwlock);
  }
}

void* tfn2(void* arg) {
  int t;
  int i = static_cast<int>(reinterpret_cast<intptr_t>(arg));
  for (;;) {
    sleep(2);
    pthread_rwlock_rdlock(&rwlock);
    printf("I'am reader: %d, read: %d\n", i, count);
    pthread_rwlock_unlock(&rwlock);
  }
}

int main() {
  int ret;
  srand(time(nullptr));
  pthread_t tid;
  pthread_attr_t attr;

  ret = pthread_rwlock_init(&rwlock, nullptr);
  if (ret != 0) {
    sys_error("rwlock", ret);
  }
  ret = pthread_attr_init(&attr);
  if (ret != 0) {
    sys_error("pthread attr init", ret);
  }
  ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  if (ret != 0) {
    sys_error("pthread setattr", ret);
  }
  
  // read
  int i;
  for (int i = 0; i < 10; i++) {
    void* id = reinterpret_cast<void*>(static_cast<intptr_t>(i));
    if (i < 2) {
      ret = pthread_create(&tid, &attr, tfn, id);
    } else {
      ret = pthread_create(&tid, &attr, tfn2, id);
    }
    if (ret != 0) {
      sys_error("pthread create", ret);
    }
  }

  ret = pthread_attr_destroy(&attr);
  if (ret != 0) {
    sys_error("pthread destory", ret);
  }

  for (;;) {
    sleep(2);
    pthread_rwlock_rdlock(&rwlock);
    printf("I'am main: %lu, read: %d\n", pthread_self(), count);
    pthread_rwlock_unlock(&rwlock);
  }

  ret = pthread_rwlock_destroy(&rwlock);
  if (ret != 0) {
    sys_error("destory rwlock", ret);
  }

  return 0;
}