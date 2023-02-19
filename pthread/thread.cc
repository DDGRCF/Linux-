#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

void sys_error(const char* msg) {
  perror(msg);
  exit(1);
}

struct thrd {
  int var;
  char str[256];
};

void* tfn(void *arg) {
  printf("thread: pid=%d, tid=%lu\n", getpid(), pthread_self());
  return nullptr;
}

void* tfn2(void *arg) {
  struct thrd* tval;
  int index = static_cast<int>(reinterpret_cast<intptr_t>(arg));
  tval = static_cast<struct thrd*>(malloc(sizeof(*tval)));
  tval->var = index;
  strcpy(tval->str, "hello thread");
  return tval;
}

int main() {
  pthread_t tid;
  printf("main: pid=%d, tid=%lu\n", getpid(), pthread_self());

  int ret = pthread_create(&tid, nullptr, tfn, nullptr);
  if (ret != 0) {
    perror("pthread_create error");
  }

  const int kNumThread = 10;
  pthread_t tids[kNumThread]{0};

  int i;
  for (i = 0; i < kNumThread; i++) {
    ret = pthread_create(&tid, nullptr, tfn2, 
        reinterpret_cast<void*>(static_cast<intptr_t>(i)));
    tids[i] = tid;
  }


  for (int j = 0; j < kNumThread; j++) {
    struct thrd* retval;
    pthread_join(tids[j], reinterpret_cast<void**>(&retval));
    printf("child thread exit with var=%d, str=%s\n", retval->var, retval->str);
    free(retval);
  }
  return 0;
}

