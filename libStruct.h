#ifndef LIBSTRUCT_H
#define LIBSTRUCT_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>

#define DOMAIN "localhost"
#define BUFFER_SIZE 4096

// Definim un tip pentru structura state
struct State {
    int smtp_socket_fd;
    int imap_socket_fd;
};

// Declaram variabila state
extern struct State state;

struct session {
    char sender_domain[100];
    char *recipient;
    char *sender;
    char *data;
};

#endif // LIBSTRUCT_H
