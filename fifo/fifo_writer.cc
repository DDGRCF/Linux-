#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


int main() {
  int ret;
  const char* path = "resource/testfifo";
  struct stat st_buf;
  ret = stat(path, &st_buf); 
  if (ret == -1 && errno == ENOENT) {
    printf("fifo don't exist! create... %s\n", path);
    ret = mkfifo(path, 0664);
    if (ret == -1) {
      perror("mkfifo error");
      exit(1);
    }
  } 
  int fd = open(path, O_WRONLY);
  if (fd < 0) {
    perror("open file");
    exit(1);
  }
  char buf[256]{0};
  int i = 0;
  while (1) {
    snprintf(buf, sizeof(buf), "I'am writer, write %d times to client\n", i++);
    ret = write(fd, buf, strlen(buf));
    printf("I'am writer! write %d bytes\n", ret);
    sleep(1);
  }
  return 0;
}