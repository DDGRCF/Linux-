#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main() {
  int fd, n;
  char msg[] = "It's is a test sentence";
  char ch;
  const char* filepath = "resource/test.txt";
  fd = open(filepath, O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    fprintf(stderr, "open %d fail, erron = %d:%s\n", fd, errno, strerror(errno));
  }
  n = write(fd, msg, strlen(msg)); // 换行符没有输入进去
  lseek(fd, 0, SEEK_SET);
  int offset;
  while ((n = read(fd, &ch, 1))) {
    if (n < 0) {
      perror("read error");
      goto end;
    }
    write(STDOUT_FILENO, &ch, n);
  }
  write(STDOUT_FILENO, "\r\n", 2);
  lseek(fd, 0, SEEK_SET);
  offset = lseek(fd, 0, SEEK_END);
  printf("file size: %d byte\n", offset);
end:
  close(fd);
  return 0;
}