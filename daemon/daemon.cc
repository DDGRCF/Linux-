#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void sys_error(const char* msg) {
  perror(msg);
  exit(1);
}

int main() {
  int ret, fd;
  pid_t pid, spid;
  pid = fork();
  if (pid == -1) {
    sys_error("fork");
  } 
  if (pid > 0) { exit(0); }
  spid = setsid();
  if (spid == -1) {
    sys_error("setsid");
  }
  chdir("/home/r");
  umask(002);
  close(STDIN_FILENO);
  fd = open("/dev/null", O_RDWR);
  if (fd == -1) {
    sys_error("open file");
  }

  fd = dup2(fd, STDOUT_FILENO);
  if (fd == -1) {
    sys_error("dup2 out");
  }
  fd = dup2(fd, STDERR_FILENO);
  if (fd == -1) {
    sys_error("dup2 error");
  }

  while(true);
}
