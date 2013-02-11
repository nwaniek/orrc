#include "or.h"
#include <sockutil.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

void
or_reset (int sockfd) {
	// reset the omnibot, wait some seconds
	char *reset = "R\n";
	Send(sockfd, reset, strlen(reset), 0);
	sleep(2);
}

void
or_disable_vcontrol (int sockfd) {
	char *disable = "\n";
	Send(sockfd, disable, strlen(disable), 0);
	sleep(1);

	// disable velocity control
	char *disable_v = "!V-\n";
	Send(sockfd, disable_v, strlen(disable_v), 0);
	sleep(1);
}

void
or_set_cycle_length (int sockfd, int c) {
	char buf[28];
	snprintf(buf, 28, "!C=%d\n", c);
	Send(sockfd, buf, strlen(buf), 0);
	printf("%s", buf);
}

void
or_send_cmd (int sockfd, char *cmd) {
	if (!cmd) return;

	size_t l = strlen(cmd);
	Send(sockfd, cmd, l, 0);
	if (cmd[l-1] == '\n') return;

	// prepend the command with \n
	char buf = '\n';
	Send(sockfd, &buf, 1, 0);
}
