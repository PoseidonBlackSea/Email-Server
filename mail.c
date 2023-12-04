#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define DOMAIN "mta.test.org"
#define BUFFER_SIZE 409

struct {
    char *domain;
    int socket_fd;
} state;


void init_socket();
void smtp_session(int client_socket);

int main(int argc, char *argv[])
{
    state.domain = DOMAIN;

    int socket_desc, client_sock, client_size;
    init_socket();    

    while(1) {
       
        struct sockaddr_in client_addr;
        client_size = sizeof(client_addr);
        client_sock = accept(state.socket_fd, (struct sockaddr*)&client_addr, &client_size);
        
        if (client_sock < 0){
            fprintf(stderr, "Can't accept\n");
            return (EXIT_FAILURE);
        }
        fprintf(stdout, "Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        
        smtp_session(client_sock);
    }

    return 0;
}

void init_socket()
{
    struct sockaddr_in server_addr;
    int rc, socket_desc;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    rc = bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if (rc < 0){
        close(socket_desc);
        printf("Couldn't bind to the port\n");
        exit (EXIT_FAILURE);
    }

    rc = listen(socket_desc, 10);

    if (rc < 0) {
        fprintf(stderr, "Failed to listen to the socket!\n");
        exit (EXIT_FAILURE);
    }

    state.socket_fd = socket_desc;

    return;

}

void smtp_session(int client_socket)
{
    char send_buffer[BUFFER_SIZE];
    char recv_buffer[BUFFER_SIZE];
    int rc;
    int socketfd = client_socket;

    sprintf(send_buffer, "220");
    rc = send(socketfd, send_buffer, strlen(send_buffer), 0);
    if (rc == -1) {
        fprintf(stderr, "ERROR at trying to send the GREETING\n");
        exit (EXIT_FAILURE);
    }

    while(1) {
        rc =  recv(socketfd, recv_buffer, BUFFER_SIZE, 0);

        if (rc == -1)       
    }

    
}