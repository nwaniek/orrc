#ifndef __OR_H__A09C41D5_AF4A_43DA_ACE9_E1AB31F2478E
#define __OR_H__A09C41D5_AF4A_43DA_ACE9_E1AB31F2478E

void or_reset(int sockfd);
void or_disable_vcontrol(int sockfd);
void or_set_cycle_length(int sockfd, int c);
void or_send_cmd(int sockfd, char *cmd);

#endif /* __OR_H__A09C41D5_AF4A_43DA_ACE9_E1AB31F2478E */

