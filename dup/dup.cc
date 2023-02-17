#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char** argv) {
  if (argc != 2) {
    perror("input param error");
  }
  int fd = open(argv[1], O_RDWR | O_CREAT, 0644);
  int newfd = dup(fd);
  char buf[256] = "I'am rcf\n";
  printf("oldfd: %d, newfd: %d\n", fd, newfd);
  lseek(fd, 0, SEEK_END);
  int ret = write(newfd, buf, strlen(buf));
  printf("write newfd: %d: %d bytes\n", newfd, ret);
  close(fd);
  close(newfd);
  return 0;
}