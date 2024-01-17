#ifndef IMAP_H
#define IMAP_H

#include "libStruct.h"
#include <stdbool.h>

#define PORT_IMAP 143

#define MAX_MESSAGES 100
#define MAX_MESSAGE_SIZE 1024

void handle_login(char *send_buffer);
void handle_select(char *mailbox, char *send_buffer);
void handle_fetch(int message_number, char *send_buffer, char *current_mailbox);
void handle_capability(char *send_buffer);


void init_socket_imap();
void *imap_session(void *parametrii);

#endif // IMAP_H