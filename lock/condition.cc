#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

class Factory {
 public:
  Factory() : dyhead_(new Product), size_(0) {}

  ~Factory() {
    Product* node = dyhead_;
    while (node != nullptr) {
      Product* tmp = node->next;
      delete node;
      node = tmp;
      size_--;
    }
    printf("调用factory的析构函数:%d\n", size_);
  }

  bool IsEmpty() { return size_ == 0; }

  void AddProduct(int num) {
    Product* new_node = new Product(num);
    Product* node = dyhead_;
    while (node->next != nullptr) {
      node = node->next;
    }
    node->next = new_node;
    size_++;
  }

  int DelProduct() {
    if (size_ < 1) {
      return -1;
    }
    Product* node = dyhead_->next;
    dyhead_->next = node->next;
    int num = node->num;
    delete node;
    size_--;
    return num;
  }

  int get_size() { return size_; }

  struct Product {
    Product() : next(nullptr), num(0) {}

    Product(int num) : next(nullptr), num(num) {}

    Product(int num, Product* next) : next(next), num(num) {}

    Product* next;
    int num;
  };

 private:
  Product* dyhead_;
  int size_;
};

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void sys_error(const char* msg, int ret) {
  fprintf(stderr, "%s, errno=%d: %s\n", msg, ret, strerror(ret));
  exit(1);
}

void* consumer(void* arg) {
  Factory* factory = static_cast<Factory*>(arg);
  while (true) {
    pthread_mutex_lock(&mutex);
    while (factory->IsEmpty()) {
      pthread_cond_wait(&cond, &mutex);
    }
    int num = factory->DelProduct();
    pthread_mutex_unlock(&mutex);
    if (num < 0) {
      printf("--Consumer: %lu, Get error", pthread_self());
    } else {
      printf("--Consumer: %lu, consume %d, %d\n", pthread_self(), num,
             factory->get_size());
    }
    sleep(rand() % 5);
  }
  return nullptr;
}

void* producter(void* arg) {
  Factory* factory = static_cast<Factory*>(arg);
  while (true) {
    pthread_mutex_lock(&mutex);
    int num = rand() % 1000 + 1;
    factory->AddProduct(num);
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);
    printf("--Producter: %lu, product %d, %d\n", pthread_self(), num,
           factory->get_size());
    sleep(rand() % 5);
  }
  return nullptr;
}

void handle_quit(int num) {
  printf("get: %d, quit\n", num);
  exit(1);
}

Factory factor;

int main() {
  signal(SIGQUIT, handle_quit);
  srand(time(nullptr));
  pthread_t tid;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  int ret;
  for (int i = 0; i < 5; i++) {
    ret = pthread_create(&tid, &attr, producter, &factor);
    if (ret == -1) {
      sys_error("pthread create", ret);
    }
  }

  for (int i = 0; i < 20; i++) {
    ret = pthread_create(&tid, &attr, consumer, &factor);
    if (ret == -1) {
      sys_error("pthread create", ret);
    }
  }

  pthread_attr_destroy(&attr);
  printf("I'am main: %lu, begin mode...\n", pthread_self());
  while (true) {}
  pthread_exit(static_cast<void*>(0));
}
