#ifndef IMAP_H
#define IMAP_H

#include "libStruct.h"

#define PORT_IMAP 143

void init_socket_imap();
void *imap_session(void *parametrii);

#endif // IMAP_H