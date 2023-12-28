#include "imap.h"
#include "smtp.h"

int main(int argc, char *argv[])
{
    int client_sock, client_size;
	

    init_socket_smtp();
    init_socket_imap();

    while(1) {
        struct sockaddr_in client_addr;
        client_size = sizeof(client_addr);
        
        client_sock = accept(state.smtp_socket_fd, (struct sockaddr*)&client_addr, &client_size);
        if (client_sock >= 0){
            fprintf(stdout, "SMTP Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            pthread_t thread;
            int *client_socket_ptr = malloc(sizeof(int));
            *client_socket_ptr=client_sock;
            pthread_create(&thread, NULL, smtp_session, (void *)client_socket_ptr);
        } else {
            //Daca nu este Conexiune SMTP Verificam pentru IMAP
            client_sock = accept(state.imap_socket_fd, (struct sockaddr*)&client_addr, &client_size);
            if (client_sock >=0){
                fprintf(stdout, "IMAP Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                pthread_t thread;
                int *client_socket_ptr = malloc(sizeof(int));
                *client_socket_ptr = client_sock;
                pthread_create(&thread, NULL, imap_session, (void *)client_socket_ptr);
            } else {
                fprintf(stderr, "Eroare acceptand Conexiunea.\n");
                return (EXIT_FAILURE);
            }
        }
    }

    return 0;
}
