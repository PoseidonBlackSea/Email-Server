#ifndef IMAP_H
#define IMAP_H

#include "libStruct.h"
#include <stdbool.h>

#define PORT_IMAP 143

#define MAX_MESSAGES 100
#define MAX_MESSAGE_SIZE 1024

void handle_login(char *recv_buff);
void handle_select(char *mailbox, char *recv_buff);
void handle_fetch(char *recv_buffer, int scoket);
void handle_capability(char *recv_buffer, int socket);
void handle_authenticate(char *recv_buff, int socket);

void init_socket_imap();
void *imap_session(void *parametrii);

#endif // IMAP_H