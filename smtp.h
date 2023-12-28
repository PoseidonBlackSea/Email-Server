#ifndef SMTP_H
#define SMTP_H

#include "libStruct.h"

#define PORT_SMTP 25

void init_socket_smtp();
void *smtp_session(void *parametrii);

#endif // SMTP_H