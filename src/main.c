#define _POSIX_C_SOURCE 200809L
#include <or.h>
#include <test/rate.h>
#include <test/poti.h>
#include <sockutil.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <pthread.h>




void* sock_reader(void *ptr);


void*
sock_reader (void *ptr) {
	if (!ptr) return NULL;
	int sockfd;
	if ((sockfd = *(int*)ptr) == -1) return NULL;

	FILE *f = fopen("streamdump", "w");
	if (!f) {
		perror("fopen");
		return NULL;
	}

	char buf[128];
	for (;;) {
		memset(buf, 0, sizeof(buf));
		int r = recv(sockfd, buf, sizeof(buf)-1, 0);
		switch (r) {
		case -1: // error
			perror("recv");
			fprintf(f, "recv: Error\n");
			fclose(f);
			return NULL;
		case 0:  // orderly shutdown
			fprintf(f, "orderly shutdown\n");
			break;
		default:
			fprintf(f, "%s", buf);
			fflush(f);
		}
	}
	fclose(f);
	return NULL;
}

void
cli (int sockfd) {
	or_reset(sockfd);
	for (;;) {
		char *cmd = readline("> ");
		if (!strcmp(cmd, "exit")) return;
		or_send_cmd(sockfd, cmd);
		free(cmd);
	}
}

#define MODE_CLI   0
#define MODE_POTI  1
#define MODE_RATE  2
#define MODE_STD   3

int
main (int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "usage: orrc <IP:port> [cli/poti/rate]\n");
		return EXIT_FAILURE;
	}

	int mode = MODE_CLI;
	if (argc > 2) {
		if (!strcmp(argv[2], "cli"))
			mode = MODE_CLI;
		else if (!strcmp(argv[2], "poti"))
			mode = MODE_POTI;
		else if (!strcmp(argv[2], "rate"))
			mode = MODE_RATE;
		else if (!strcmp(argv[2], "std"))
			mode = MODE_STD;
		else {
			fprintf(stderr, "Unknown mode\n");
			return EXIT_FAILURE;
		}
	}

	char *pch;
	char ip[16], port[6];
	pch = strtok(argv[1], ":");
	strncpy(ip, pch, 15);
	pch = strtok(NULL, ":");
	strncpy(port, pch, 5);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket");
		return EXIT_FAILURE;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));

	int i = inet_pton(AF_INET, ip, &addr.sin_addr);
	if (i <= 0) {
		if (i == 0)
			fprintf(stderr, "IP not in presentation format\n");
		else
			perror("inet_pton");
		return EXIT_FAILURE;
	}

	if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr))) {
		perror("connect");
		return EXIT_FAILURE;
	}
	// fcntl(sockfd, F_SETFL, O_NONBLOCK);

	pthread_t readt;
	pthread_create(&readt, NULL, sock_reader, (void*)&sockfd);

	switch (mode) {
	case MODE_CLI:
		cli(sockfd);
		break;
	case MODE_POTI:
		test_poti(sockfd);
		break;
	case MODE_RATE:
		test_rate(sockfd);
		break;
	case MODE_STD:
		test_poti_std(sockfd);
		break;
	default:
		fprintf(stderr, "Unknown mode\n");
		return EXIT_FAILURE;
	}

	close(sockfd);
	// pthread_kill(readt, 0);a
	pthread_join(readt, NULL);
	return EXIT_SUCCESS;
}

