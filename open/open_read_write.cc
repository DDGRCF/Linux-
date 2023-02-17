#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

const int kBufSize = 2;

int main() {
	const char* filepath = "resource/test.txt";
	const char* dstpath = "resource/dst.txt";
	int fd = open(filepath, O_RDONLY);
	int fd2 = open(dstpath, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd == -1) {
		printf("fd = %d, erron = %d:%s\n", fd, errno, strerror(errno));
		return -1;
	}
	int ret = 0;
	void* buf[kBufSize];
	while ((ret = read(fd, buf, kBufSize)) > 0) {
		if (ret == -1) {
			// 当非阻塞才有用这里是阻塞
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				printf("read in non-blocking manner, erron = %d:%s\n", errno, strerror(errno));
			} else {
				printf("read fail, erron = %d:%s\n", errno, strerror(errno));
				break;
			}
		}
		ret = write(fd2, buf, ret);
		printf("write the %d byte data\n", ret);
	}
	close(fd);
	close(fd2);
	return 0;
}
