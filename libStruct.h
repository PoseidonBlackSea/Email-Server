#ifndef LIBSTRUCT_H
#define LIBSTRUCT_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
#include <mysql.h>


#define DOMAIN "localhost"
#define BUFFER_SIZE 4096

struct State {
    int smtp_socket_fd;
    int imap_socket_fd;
};

extern struct State state;

struct session_smtp {
    char recipient[256];
    char sender[256];
    char content[BUFFER_SIZE];
};

struct MailDB{
	char *server;
	char *user;
	char *password;
	char *database;
};

extern struct MailDB mail_db;

#endif  //LIBSTRUCT_H
