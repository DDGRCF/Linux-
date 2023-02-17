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

  int ret = 0;
  int pipefd[2];
  pid_t cpid;
  char buf;
  const char* path = argv[1];

  if (pipe(pipefd) == -1) {
    perror("pipe");  
    ret = EXIT_FAILURE;
    goto END;
  }

  cpid = fork();
  if (cpid == -1) {
    perror("fork");
    ret = EXIT_FAILURE;
    goto END;
  }

  if (cpid == 0) {
    close(pipefd[1]); // close write
    int newfd = dup2(pipefd[0], STDIN_FILENO);
    if (newfd == -1) {
      ret = EXIT_FAILURE;
      goto CHILDEND;
    }
    execlp("wc", "wc", "-l", nullptr);
CHILDEND:
    close(pipefd[0]);
  } else {
    close(pipefd[0]); // close read
    int newfd = dup2(pipefd[1], STDOUT_FILENO);
    if (newfd == -1) {
      ret = EXIT_FAILURE;
      goto PARENTEND;
    }
    execlp("ls", "ls", path, nullptr);
PARENTEND:
    close(pipefd[1]);
  }
END:
  close(pipefd[0]);
  close(pipefd[1]);
  return ret;
}