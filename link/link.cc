#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
  int fd, ret;
  const char* file_path = "resource/test.txt";
  fd = open(file_path, O_RDWR | O_CREAT | O_TRUNC, 0644);
  if (fd < 0) {
    perror("open file error");
    exit(1);
  }

  ret = unlink(file_path);
  if (ret < 0) {
    perror("unlink file error");
    exit(1);
  }


  const char* text = "I'am rcf\ntest";
  char buf[256]{0};

  ret = write(fd, text, strlen(text));
  if (ret < 0) {
    perror("write error");
    goto end;
  }
  printf("write %d: %s\n", fd, text);

  lseek(fd, 0, SEEK_SET);
  ret = read(fd, buf, sizeof(buf));
  if (ret < 0) {
    perror("read error");
    goto end;
  }
  printf("read %d: %s\n", fd, buf);

  char a; 
  while ((a = getchar()) != 'q') {
    printf("%c", a);
  }

end:
  close(fd);
  return 0;
}