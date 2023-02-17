#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv) {
  if (argc != 2) {
    perror("input parameters error");
    exit(EXIT_FAILURE);
  }

  int ret;
  int pipefd[2];
  pid_t cpid;
  char buf;
  const char* filepath = argv[1];

  if (pipe(pipefd) == -1) {
    perror("pipe");  
    exit(EXIT_FAILURE);
  }

  cpid = fork();
  if (cpid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (cpid == 0) {
    close(pipefd[0]); // close read
    char buf[256]{0}; 
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
      perror("open file");
      goto CHILDEND;
    }
    while ((ret = read(fd, buf, sizeof(buf) - 1)) > 0) { // 减一要加入\0字符结束
      buf[ret] = '\0'; 
      ret = write(pipefd[1], buf, ret + 1); // 加一是因为加入了\0
      printf("I'am child: %d, write %d bytes\n", getpid(), ret + 1);
      sleep(1);
    }
CHILDEND:
    close(pipefd[1]);
  } else {
    close(pipefd[1]); // close write
    char buf[256]{0};
    while ((ret = read(pipefd[0], buf, sizeof(buf))) > 0) {
      printf("I'am parent: %d, read %d from %s child\n", getpid(), ret, buf);
    }
PARENTEND:
    close(pipefd[0]);
  }
  return 0;
}