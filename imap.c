#include "imap.h"

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

void *imap_session(void *parametrii){
    int k;

    int client_socket_fd = *((int *)parametrii);
    int imap_socket_fd = state.imap_socket_fd;

    free(parametrii);

    struct session session;

    char send_buffer[BUFFER_SIZE];
    char recv_buffer[BUFFER_SIZE];

    snprintf(send_buffer,sizeof(send_buffer),"* OK IMAP Server Ready\r\n");
    k = send(client_socket_fd,send_buffer,strlen(send_buffer), 0);
    
    if (k == -1) {
        fprintf(stderr, "ERROR Imap Server NOT Ready\n");
        exit (EXIT_FAILURE);
    }

    while(1){
        //Primim comanda clientului
        int bytes_received=recv(client_socket_fd,recv_buffer,sizeof(recv_buffer), 0);
        if(bytes_received <= 0){
            printf("Client disconnected.\n");
            break;
        }

        //Printam Comanda clientului
        recv_buffer[bytes_received]= '\0';
        printf("Received: %s", recv_buffer);

        //Echo Comanda inapoi la client
        snprintf(send_buffer,sizeof(send_buffer),"%s",recv_buffer);
        k = send(client_socket_fd,send_buffer,strlen(send_buffer), 0);
     
        if (k == -1) {
            fprintf(stderr, "ERROR Imap Server NOT Ready\n");
            exit (EXIT_FAILURE);
        }
    }

    close(client_socket_fd);

}