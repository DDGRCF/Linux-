#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

void sys_error(const char* msg) {
  perror(msg);
  exit(1);
}

int main() {
  pid_t pid;
  int ret, fd;
  const char* path = "resource/text";
  fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    sys_error("open file");
  }
  const int size = sizeof(int);
  if ((ret = ftruncate(fd, size)) == -1) {
    sys_error("ftruncate");
  }
  int len;
  if ((len = lseek(fd, 0, SEEK_END)) == -1) {
    sys_error("lseek");
  }
  int* p = static_cast<int*>(
      mmap(nullptr, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
  if (p == MAP_FAILED) {
    sys_error("mmap");
  }
  memset(p, 0, sizeof(int));
  close(fd);
  pid = fork();
  if (pid == -1) {
    sys_error("fork");
  }
  if (pid == 0) {
    *p = 666;
    printf("I'am child: %d, I transfer: %d\n", getpid(), *p);  //
  } else if (pid > 0) {
    sleep(1);
    printf("I'am parent: %d, I get %d\n", getpid(), *p);
    if (munmap(p, len) == -1) {
      sys_error("munmap");
    }
  }
  return 0;
}
