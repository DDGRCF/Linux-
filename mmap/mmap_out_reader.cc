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
    (*this)++;
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
  if ((fd = open(path, O_RDONLY)) == -1) {
    sys_error("open file");
  };

  void* mm = mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, 0);
  if (mm == MAP_FAILED) {
    sys_error("mmap");
  }
  close(fd);
  srand(time(nullptr));
  while (1) {
    sleep(rand() % 5);
    memcpy(&tt, mm, size);
    printf("I'am reader, x: %d, y: %d\n", tt.get_x(), tt.get_y());
  }

  if ((ret = munmap(mm, size)) == -1) {
    sys_error("munmap");
  }
  return 0;
}
