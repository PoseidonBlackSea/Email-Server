#include "imap.h"

void handle_login(char *send_buffer) {
    // Implementează logica pentru gestionarea comenzii LOGIN
    // Exemplu simplu:
    printf("LOGIN\n");
    snprintf(send_buffer, 256, "A1 OK LOGIN completed\r\n");
}
void handle_select(char *mailbox, char *send_buffer) {
    // Implementează logica pentru gestionarea comenzii SELECT
    // Exemplu simplu:
    printf("SELECT %s\n", mailbox);
    snprintf(send_buffer, 256, "A2 OK [READ-WRITE] SELECT completed\r\n");
}

void handle_fetch(int message_number, char *send_buffer, char *current_mailbox) {
    // Implementeaza logica pentru comanda FETCH aici
}\

void handle_capability(char *recv_buff, int socket) {
    // Implementează logica pentru gestionarea comenzii CAPABILITY
    // Exemplu simplu:
    //snprintf(send_buffer, 256, "* OK [CAPABILITY IMAP4rev1 LITERAL+ SASL-IR LOGIN-REFERRALS ID ENABLE IDLE AUTH=PLAIN AUTH=LOGIN] IMAP/POP3 ready - us11-011mip \r\n");
    char send_buffer[256];
    memcpy(send_buffer,"* OK [CAPABILITY IMAP4rev1 AUTH=PLAIN ]\r\n", sizeof(send_buffer));
    send(socket, send_buffer, strlen(send_buffer), 0);


    memset(send_buffer, '\0', 256);
    memcpy(send_buffer, recv_buff, 2);
    memcpy(send_buffer+2, " OK CAPABILITY completed \r\n", 29);
    send(socket, send_buffer, sizeof(send_buffer), 0);
}

void handle_authenticate(char *recv_buff, int socket) {
    char send_buffer[256];
    memcpy(send_buffer, "+ \r\n", sizeof(send_buffer));
    send(socket, send_buffer, strlen(send_buffer), 0);

    char aux[512];
    recv(socket, aux, sizeof(aux), 0);
    
    memset(send_buffer, '\0', sizeof(send_buffer));
    memcpy(send_buffer, recv_buff, 2);
    memcpy(send_buffer+2, " OK AUTHENTICATE completed \r\n", 31);
    send(socket, send_buffer, sizeof(send_buffer), 0);

}

void init_socket_imap(){
    struct sockaddr_in server_addr;
    int rc, socket_desc;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_IMAP);
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

    printf("IMAP Server listening on port %d...\n", PORT_IMAP);

    state.imap_socket_fd = socket_desc;

    return;
}

void *imap_session(void *parametrii) {
    // MYSQL *conn;
	// MYSQL_RES *res;
	// MYSQL_ROW row;

    // onn = mysql_init(NULL);

    char recv_buffer[BUFFER_SIZE];
    char send_buffer[BUFFER_SIZE];
    // int k;
    int client_socket_fd = *((int *)parametrii);
    free(parametrii);

    // struct session session;

    char mailbox[BUFFER_SIZE];
    int selected = 0;
    char current_mailbox[BUFFER_SIZE];

    snprintf(send_buffer, sizeof(send_buffer), "* OK IMAP Server Ready\r\n");
    int k = send(client_socket_fd, send_buffer, strlen(send_buffer), 0);

    if (k == -1) {
        fprintf(stderr, "ERROR Imap Server NOT Ready\n");
        exit(EXIT_FAILURE);
    }

    // if(!mysql_real_connect(conn,mail_db.server,mail_db.user,mail_db.password,mail_db.database,0,NULL,0))
    // {
    //     fprintf(stderr,"%s\n", mysql_error(conn));
    //     exit(1);
    // }

    while (1) {
        int bytes_received = recv(client_socket_fd, recv_buffer, sizeof(recv_buffer), 0);
        fprintf(stderr, "%s\n", recv_buffer);
        if (bytes_received <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        recv_buffer[bytes_received] = '\0';
        printf("Comanda primita de la client: %s", recv_buffer);

        if (strstr(recv_buffer, "capability") != NULL) {
            handle_capability(recv_buffer, client_socket_fd);
        } else if (strstr(recv_buffer, "LOGIN") != NULL) {
            handle_login(send_buffer);
        } else if (strstr(recv_buffer, "SELECT") != NULL) {
            char mailbox[50];
            sscanf(recv_buffer, "SELECT %s", mailbox);
            handle_select(mailbox, send_buffer);
        } else if (strstr(recv_buffer, "FETCH") != NULL) {
           //Restul codului pentru gestionarea comenzii FETCH
           //...
        }else if(strstr(recv_buffer, "authenticate") != NULL) {
            handle_authenticate(recv_buffer, client_socket_fd);
        } else if (strstr(recv_buffer, "LOGOUT") != NULL) {
           //Restul codului pentru gestionarea comenzii LOGOUT
           //...
        } else {
            snprintf(send_buffer, 256, "A99 NO Unknown command\r\n");
        }
        if (k == -1) {
            fprintf(stderr, "ERROR Sending response\n");
            exit(EXIT_FAILURE);
        }
    }

    close(client_socket_fd);

    return NULL;
}