#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void sys_error(const char* msg) {
  perror(msg);
  exit(1);
}

class Test {
 public:
  Test(int x, int y) : x_(x), y_(y) {}

  int get_x() { return x_; }

  int get_y() { return y_; }

  Test operator++() {
    this->x_++;
    this->y_++;
    return *this;
  }

  Test operator++(int) {
    Test tmp = *this;
    ++*this;
    return tmp;
  }

 private:
  int x_, y_;
};

int main() {
  int ret, fd;
  const char* path = "resource/text";
  Test tt{0, 0};
  const int size = sizeof(tt);
  if ((fd = open(path, O_RDWR | O_CREAT, 0644)) == -1) {
    sys_error("open file");
  };

  if ((ret = ftruncate(fd, size)) == -1) {
    sys_error("ftruncate file");
  }

  int len;
  if ((len = lseek(fd, 0, SEEK_END)) == -1) {
    sys_error("lseek file");
  }

  void* mm = mmap(nullptr, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (mm == MAP_FAILED) {
    sys_error("mmap");
  }
  close(fd);
  srand(time(nullptr));
  while (1) {
    printf("I'am writer, x: %d, y: %d\n", tt.get_x(), tt.get_y());
    memcpy(mm, &tt, sizeof(tt));
    tt++;
    sleep(rand() % 5);
  }
  return 0;
}
