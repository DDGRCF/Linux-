#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void sys_error(const char* msg) {
  perror(msg);
  exit(1);
}

int main() {
  const int kSize = 2 << 14;
  const char* shm_name = "shm_test";
  int fd = shm_open(shm_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
  shm_unlink(shm_name);
  if (fd == -1) {
    sys_error("shm open");
  }

  int ret = ftruncate(fd, kSize);
  if (ret == -1) {
    sys_error("ftruncate");
  }

  void* memory =
      mmap(nullptr, kSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (memory == MAP_FAILED) {
    sys_error("munmap");
  }

  close(fd);
  pid_t pid = fork();
  if (pid == -1) {
    sys_error("fork");
  }

  if (pid > 0) {
    printf("into main process %d...\n", getpid());
    snprintf(static_cast<char*>(memory), kSize,
             "client: %d, THE CONTEXT FROM MEMORY", getpid());
    sleep(1);
  } else {
    sleep(1);
    printf("the message from memory: %s\n", static_cast<const char*>(memory));
    sleep(1);
    ret = munmap(memory, kSize);
    if (ret == -1) {
      sys_error("munmap");
    }
  }

  return 0;
}
