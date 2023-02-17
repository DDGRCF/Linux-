#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>


int read_dir(const char* dirpath, const char* prefix = "") {
  int ret;
  struct stat st_buf;
  ret = stat(dirpath, &st_buf);
  if (ret < 0) {
    perror("read dirpath info error");
    return -1;
  }

  char newprefix[BUFSIZ];
  strcpy(newprefix, prefix);
  strcat(newprefix, "  ");

  if (S_ISDIR(st_buf.st_mode)) {
    DIR* dp;
    struct dirent* sdp;
    dp = opendir(dirpath);
    if (!dp) {
      fprintf(stderr, "open dir %s error, errno=%d:%s\n", 
          dirpath, errno, strerror(errno));
      return -1;
    }

    printf("%s%s\n", prefix, dirpath);
    while ((sdp = readdir(dp)) != nullptr) {
      if (strcmp(".", sdp->d_name) && strcmp("..", sdp->d_name)) {
        char newpath[BUFSIZ];
        snprintf(newpath, sizeof(newpath), "%s/%s", dirpath, sdp->d_name);
        read_dir(newpath, newprefix); 
      }
    }
    closedir(dp);
  } else {
    printf("%s%s\n", prefix, dirpath);
  }
  return 0;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    perror("input param error");
    exit(1);
  }
  int ret = read_dir(argv[1]);
  if (ret < 0) {
    perror("read dir error");
  }

  return 0;
}