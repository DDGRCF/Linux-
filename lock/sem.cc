#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
const int kNum = 5;

int queue[kNum]{0};
sem_t blanket_num;
sem_t product_num;

void sys_error(const char* msg, int ret) {
  fprintf(stderr, "%s, errno=%d: %s\n", msg, ret, strerror(ret));
  exit(1);
}

void* consumer(void* arg) {
  int i = 0;
  for (;;) {
    sem_wait(&product_num);
    printf("--Consumer: %d\n", queue[i]);
    queue[i] = 0;
    sem_post(&blanket_num);
    sleep(rand() % 3);
  }
  return nullptr;
}

void* producter(void* arg) {
  int i = 0;
  for (;;) {
    sem_wait(&blanket_num);
    queue[i] = rand() % 1000 + i;
    printf("--Product: %d\n", queue[i]);
    sem_post(&product_num);
    i = (i + 1) % kNum;
    sleep(1);
  }
  return nullptr;
}

int main() {
  int ret;
  ret = sem_init(&blanket_num, 0, kNum);
  if (ret == -1) {
    sys_error("sem_init", ret);
  }
  ret = sem_init(&product_num, 0, 0);
  if (ret == -1) {
    sys_error("sem_init", ret);
  }
  pthread_t tid;
  ret = pthread_create(&tid, nullptr, producter, nullptr);
  if (ret == -1) {
    sys_error("pthread create error", ret);
  }

  ret = pthread_create(&tid, nullptr, consumer, nullptr);
  if (ret == -1) {
    sys_error("pthread create error", ret);
  }

  pthread_join(tid, nullptr);
  pthread_join(tid, nullptr);

  sem_destroy(&blanket_num);
  sem_destroy(&product_num);
  return 0;
}
