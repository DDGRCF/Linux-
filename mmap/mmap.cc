#include <sys/mman.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void sys_error(const char* msg) {
  perror(msg);
  exit(1);
}

int main() {
  int fd, ret, len;
  const char* path = "resource/text";
  off_t size = 200;
  fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    sys_error("open file");
  }
  if ((ret = ftruncate(fd, size)) == -1) {
    sys_error("truncate file");
  }
  if ((len = lseek(fd, 0, SEEK_END)) == -1) {
    sys_error("lseek file");
  }
  char* p = static_cast<char *>(mmap(nullptr, len, 
      PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
  if (p == MAP_FAILED) {
    sys_error("mmap");
  }
  strcpy(p, "hello world!");
  printf("-----%s\n", p);
  if ((ret = munmap(p, len)) == -1) {
    sys_error("munmap error");
  }
  close(fd);
  
  return 0;
}