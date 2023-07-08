#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
  if (argc != 3) {
    perror("input param error");
  }
  int fd1 = open(argv[1], O_RDWR | O_CREAT, 0644);
  int fd2 = open(argv[2], O_RDWR | O_CREAT, 0644);
  int ret = dup2(fd1, fd2);
  if (ret < 0) {
    perror("dup2 error");
  }
  char buf[256] = "I'am rcf\n";
  lseek(fd2, 0, SEEK_END);
  ret = write(fd2, buf, strlen(buf));
  if (ret < 0) {
    perror("write data to file error");
  }
  ret = dup2(STDOUT_FILENO, fd1);
  if (ret < 0) {
    perror("dup2 error");
  }

  ret = write(fd1, buf, strlen(buf));
  if (ret < 0) {
    perror("write data to file error");
  }
  return 0;
}
