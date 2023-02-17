#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int main() {
  pid_t pid;
  const int kProcessNum = 20;
  int i;
  for (i = 0; i < kProcessNum; i++) {
    pid = fork();
    if (pid == -1) {
      perror("fork error");
      exit(1);
    } else if (pid == 0) {
      break;
    }
  }

  if (i != kProcessNum) { //  这里也可以根据i判断
    sleep(2);
    printf("I'am child: %d:%d\n", i, pid);
  } else {
    printf("I'am parent: %d, I'am create %d number child\n", pid, i);
  }
  return 0;
}