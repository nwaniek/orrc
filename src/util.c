#define _POSIX_C_SOURCE 200809L
#include <stdint.h>
#include "util.h"
#include <stdio.h>
#include <errno.h>
#include <time.h>


uint64_t
get_realtime_ns() {
	struct timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
		perror("clock_gettime");
		return 0;
	}
	return ts.tv_sec * 1000000000LL + ts.tv_nsec;
}
