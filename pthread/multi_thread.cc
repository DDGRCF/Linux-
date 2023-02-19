#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int var = 200;

void sys_error(const char* msg) {
  perror(msg);
  exit(1);
}

void *tfn(void *arg) {
  int i = static_cast<int>(reinterpret_cast<intptr_t>(arg));
  var /= 2;
  sleep(i);
  printf("thread: pid=%d, tid=%lu, ret=%d, var=%d\n", getpid(), pthread_self(), i, var);
  return nullptr;
}

int main() {
  int ret;
  pthread_t tid;
  printf("main: pid=%d, tid=%lu\n", getpid(), pthread_self());
  const int kNumThread = 5;
  int i;
  for (i = 0; i < kNumThread; i++) {
    // ret = pthread_create(&tid, nullptr, tfn, reinterpret_cast<void*>(i)); // 这个可以
    ret = pthread_create(&tid, nullptr, tfn, reinterpret_cast<void*>(static_cast<intptr_t>(i))); // 这个更好 
    if (ret != 0) {
      perror("pthread_create_error");
    }
  }
  // sleep(i);
  printf("I'am main, pid=%d, tid=%lu\n", getpid(), pthread_self());
  pthread_exit(nullptr);

  return 0;
}

