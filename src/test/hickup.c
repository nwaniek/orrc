#define _POSIX_C_SOURCE 200809L
#include "hickup.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <or.h>
#include <sockutil.h>
#include <unistd.h>
#include <time.h>

#define MAX_WAIT_TIME 3600
#define VEL_RANGE     65

void
test_hickup(int sockfd) {
	struct timespec t;
	struct timespec c_start, c_now;

	or_reset(sockfd);
	// or_disable_vcontrol(sockfd);

	t.tv_sec = 1;
	t.tv_nsec = 0;
	nanosleep(&t, NULL);

	if (clock_gettime(CLOCK_REALTIME, &c_start)) {
		fprintf(stderr, "EE: Could not get time\n");
		return;
	}

	char *streaming = "!I1,125,2\n";
	Send(sockfd, streaming, strlen(streaming), 0);
	nanosleep(&t, NULL);


	int i = 0, j = 0;
	for (;;) {
		char buf[64];
		snprintf(buf, 64, "!Va=%d\n", i - VEL_RANGE);
		printf("%s", buf);
		Send(sockfd, buf, strlen(buf), 0);
		i = (i + 1) % (VEL_RANGE * 2 + 1);
		j++;

		t.tv_sec = 0;
		t.tv_nsec = 500000000;
		nanosleep(&t, NULL);

		clock_gettime(CLOCK_REALTIME, &c_now);
		if (c_now.tv_sec - c_start.tv_sec > MAX_WAIT_TIME) {
			break;
		}
	}

	printf("Done testing. Commands sent: %d\n", j);
	or_reset(sockfd);
}
