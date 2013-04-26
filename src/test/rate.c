#include "rate.h"
#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <or.h>
#include <sockutil.h>
#include <string.h>


void
test_rate (int sockfd) {
	// prepare nanosleep argument (sleep 10 minutes)
	struct timespec t;
	t.tv_sec = (int)60;
	t.tv_nsec = 0;

	or_reset(sockfd);

	// enable streaming of relevant data at 250hz
	char *streaming = "!I1,125,A\n";
	Send(sockfd, streaming, strlen(streaming), 0);

	nanosleep(&t, NULL);

	// disable streaming
	char *dstreaming = "!I0,125,A\n";
	Send(sockfd, dstreaming, strlen(dstreaming), 0);
	sleep(1);
}

void
test_rate2(int sockfd) {
	struct timespec t;
	t.tv_sec = (int)60;
	t.tv_nsec = 0;

	or_reset(sockfd);
	nanosleep(&t, NULL);
	or_reset(sockfd);

	t.tv_sec = 0;
	t.tv_nsec = 100000;
	nanosleep(&t, NULL);
}
