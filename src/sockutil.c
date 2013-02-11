#include "sockutil.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>

ssize_t
Send (int sockfd, const void *buf, size_t len, int flags) {
	ssize_t s = send(sockfd, buf, len, flags);
	if (s == -1) {
		perror("send");
		exit(EXIT_FAILURE);
	}
	return s;
}
