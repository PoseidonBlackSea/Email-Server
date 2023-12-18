#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
//#include <mysql.h>

#define DOMAIN "mta.test.org"
#define BUFFER_SIZE 409

struct {
    int socket_fd;
} state;

struct session{
    char sender_domain[100];
    char *recipient;
    char *sender;
    char *data;
};

//MYSQL *conn;


void init_socket();
void *smtp_session(void *param);

int main(int argc, char *argv[])
{
    int client_sock, client_size;
	
	// char *server = "localhost";
	// char *user = "root";
	// char *password = "root"; /* set me first */
	// char *database = "mysql";

    // conn = mysql_init(NULL);

    // if (!mysql_real_connect(conn, server, user, password, 
    //                                   database, 0, NULL, 0)) {
	// 	fprintf(stderr, "%s\n", mysql_error(conn));
	// 	exit(1);
	// }

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

        pthread_t thread;
        int *client_socket_ptr = malloc(sizeof(int));
        *client_socket_ptr = client_sock;
        pthread_create(&thread, NULL, smtp_session, (void *)client_socket_ptr);
    }

    //mysql_close(conn);
    return 0;
}

void init_socket()
{
    struct sockaddr_in server_addr;
    int rc, socket_desc;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5001);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    rc = bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if (rc < 0){
        close(socket_desc);
        fprintf(stderr,"Couldn't bind to the port\n");
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

void *smtp_session(void *param)
{
    int client_socket = *((int *)param);
    free(param);

    struct session session;
    
    char send_buffer[BUFFER_SIZE];
    char recv_buffer[BUFFER_SIZE];
    char *token;
    int rc;
    int data_message = 0;

    sprintf(send_buffer, "220 \r\n");
    rc = send(client_socket, send_buffer, strlen(send_buffer), 0);
    if (rc == -1) {
        fprintf(stderr, "ERROR at trying to send the GREETING\n");
        exit (EXIT_FAILURE);
    }


    while(1) {
        rc = recv(client_socket,  recv_buffer, BUFFER_SIZE, 0);
        
        if (rc == 0) {
            fprintf(stderr, "Host closed the socket!\n");
            break;
        }
        if (rc == -1) {
            fprintf(stderr, "Error on sockett!\n");
            exit (EXIT_FAILURE);
        }
        char *eol = strstr(recv_buffer, "\n");
        eol[0] = '\0';

        fprintf(stderr, "%s - %d\n", recv_buffer, client_socket);
    if (data_message == 0) {
        
        char command[4];
        for(int i=0; i<4; i++)
            command[i] = recv_buffer[i];
        command[4] = '\0';
        
        if (strcmp(command, "HELO") == 0) {
            strcpy(session.sender_domain, recv_buffer + 5);
            
            strcpy(send_buffer, "250 OK\r\n");
            send(client_socket, send_buffer, strlen(send_buffer), 0);
        } else if (strcmp(command, "MAIL") == 0) {
            strcpy(send_buffer, "250 OK\r\n");
            send(client_socket, send_buffer, strlen(send_buffer), 0);
        } else if (strcmp(command, "RCPT") == 0) {
            strcpy(send_buffer, "250 OK\r\n");
            send(client_socket, send_buffer, strlen(send_buffer), 0);
        } else if (strcmp(command, "DATA") == 0) {
            strcpy(send_buffer, "354\r\n");
            send(client_socket, send_buffer, strlen(send_buffer), 0);
            data_message = 1;
        } else if (strcmp(command, "QUIT") == 0) {
            strcpy(send_buffer, "221\r\n");
            send(client_socket, send_buffer, strlen(send_buffer), 0);
            break;
        }
    } else if (data_message == 1) {
        if (strcmp(recv_buffer, ".") == 0) {
            strcpy(send_buffer, "250\r\n");
            send(client_socket, send_buffer, strlen(send_buffer), 0);
            data_message = 0;
        }
    }
        memset(send_buffer, '\0', BUFFER_SIZE);
    }
    
    close(client_socket);

    return ;
}
