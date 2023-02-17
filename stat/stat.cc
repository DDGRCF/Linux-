#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "input parameters error, need 2 input %d\n", argc);
    exit(1);
  }
  const char* file_path = argv[1];
  struct stat st;
  int ret = stat(file_path, &st);
  if (ret == -1) {
    perror("stat error");
    exit(1);
  }
  printf("file size: %ld\n", st.st_size);
}