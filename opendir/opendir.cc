#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int main() {
  DIR* dp;
  struct dirent* sdp;
  const char* dirpath = "resource";
  dp = opendir(dirpath);
  if (dp == nullptr) {
    perror("opendir error");
    exit(1);
  }
  while ((sdp = readdir(dp)) != nullptr) {
    if (strcmp(".", sdp->d_name) && 
        strcmp("..", sdp->d_name)) {
      printf("%s\t", sdp->d_name);
    }
  }
  printf("\n");
  closedir(dp);

  return 0;
}