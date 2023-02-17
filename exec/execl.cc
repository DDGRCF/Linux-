#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv) {
  if (argc != 2) {
    perror("input params error");
  }
  const char* path = argv[1];
  int i;
  pid_t pid; 
  for (i = 0; i < 3; i++) {
    if ((pid = fork()) == 0) { break; }
  }
  if (pid == -1) {
    perror("fork error");
    exit(1);
  }
  if (pid == 0) {
    if (i == 0) {
      int ret = execl("/bin/ls", "ls", "-l", path, nullptr);
      if (ret == -1) {
        perror("execl error");
        exit(1);
      }
    } else if (i == 1) {
      int ret = execlp("ls", "ls", "-lh", path, nullptr);
      if (ret == -1) {
        perror("execl error");
        exit(1);
      }
    } else if (i == 2) {
      char* argv[] = {"ls", "-lh", const_cast<char*>(path), nullptr};
      int ret = execvp("ls", argv);
      if (ret == -1) {
        perror("execl error");
        exit(1);
      }
    }

  } else if (i == 3) {
    printf("I'am parent : %d\n", getpid());
    sleep(2);
  }
}