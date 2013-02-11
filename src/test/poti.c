#include "poti.h"
#include <or.h>
#include <sockutil.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

void
test_poti (int sockfd) {
	or_reset(sockfd);
	or_disable_vcontrol(sockfd);

	// enable streaming of relevant data at 250hz
	char *streaming = "!I1,250,y\n";
	Send(sockfd, streaming, strlen(streaming), 0);
	sleep(1);


	/*
	// set pulse width
	int base = 0;

	for (int j = 0; j < 10; j++) {
		for (int i = 0; i <= 64; i++) {
			// increase pulse width by 10
			int pwm = i * 10 + base;
			if (pwm > 640) break;

			// send command, all wheels at the same time
			char buf[64];
			snprintf(buf, 64, "!S0=%d,1=%d,2=%d\n", pwm, 0, 0);
			printf("%s", buf);
			Send(sockfd, buf, strlen(buf), 0);

			sleep(2);
		}
		base++;
	}
	*/
	for (int c = 630; c <= 650; c++) {
		or_set_cycle_length(sockfd, c);
		for (int j = 5; j <= 11; j++) {
			int pwm = j * 10;

			// send command, all wheels at the same time
			char buf[64];
			snprintf(buf, 64, "!S0=%d,1=%d,2=%d\n", pwm, 0, 0);
			printf("%s", buf);
			Send(sockfd, buf, strlen(buf), 0);

			sleep(2);
		}
	}

	char *stop = "!S1=0,2=0,3=0\n";
	Send(sockfd, stop, strlen(stop), 0);
	sleep(1);
}


void
test_poti_std (int sockfd) {
	or_reset(sockfd);
	or_disable_vcontrol(sockfd);

	// enable streaming of relevant data at 250hz
	char *streaming = "!I1,250,y\n";
	Send(sockfd, streaming, strlen(streaming), 0);
	sleep(1);

	for (int c = 100; c <= 1000; c = c + 10) {
		or_set_cycle_length(sockfd, c);

		// pwm should reflect 100, if c = 640
		// int pwm = (int)ceilf(((float)c / 640.0f) * 100.0f);
		int pwm = c / 2;

		char buf[64];
		snprintf(buf, 64, "!S0=%d,1=%d,2=%d\n", pwm, pwm, pwm);
		printf("%s", buf);
		Send(sockfd, buf, strlen(buf), 0);

		sleep(20);
	}

	char *stop = "!S1=0,2=0,3=0\n";
	Send(sockfd, stop, strlen(stop), 0);
	sleep(1);
}
