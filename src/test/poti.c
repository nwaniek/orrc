#include "poti.h"
#include <or.h>
#include <sockutil.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define LENGTH(X) (sizeof X / sizeof X[0])

void
test_poti2(int sockfd) {
	or_reset(sockfd);
	sleep(1);


	// leave cycle duty length = 640
	for (int pw = 200; pw < 500; pw += 5) {
		char buf[64];

		// 001
		snprintf(buf, 64, "!S0=%d,1=0,2=0\n", pw);
		printf("%s", buf);
		Send(sockfd, buf, strlen(buf), 0);
		sleep(5);

		// 010
		snprintf(buf, 64, "!S0=0,1=%d,2=0\n", pw);
		printf("%s", buf);
		Send(sockfd, buf, strlen(buf), 0);
		sleep(5);

		// 100
		snprintf(buf, 64, "!S0=0,1=0,2=%d\n", pw);
		printf("%s", buf);
		Send(sockfd, buf, strlen(buf), 0);
		sleep(5);

		// 011
		snprintf(buf, 64, "!S0=%d,1=%d,2=0\n", pw, pw);
		printf("%s", buf);
		Send(sockfd, buf, strlen(buf), 0);
		sleep(5);

		// 101
		snprintf(buf, 64, "!S0=%d,1=0,2=%d\n", pw, pw);
		printf("%s", buf);
		Send(sockfd, buf, strlen(buf), 0);
		sleep(5);

		// 110
		snprintf(buf, 64, "!S0=0,1=%d,2=%d\n", pw, pw);
		printf("%s", buf);
		Send(sockfd, buf, strlen(buf), 0);
		sleep(5);

		// 111
		snprintf(buf, 64, "!S0=%d,1=%d,2=%d\n", pw, pw, pw);
		printf("%s", buf);
		Send(sockfd, buf, strlen(buf), 0);
		sleep(5);
	}

	char *stop = "!S1=0,2=0,3=0\n";
	Send(sockfd, stop, strlen(stop), 0);
	sleep(1);
	printf("done\n");
}

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

	for (int c = 500; c <= 1000; c = c + 5) {
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
