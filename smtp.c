#include "smtp.h"

void init_socket_smtp()
{
    struct sockaddr_in server_addr;
    int rc, socket_desc;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_SMTP);
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

    fprintf(stderr, "SMTP Server listening on port %d...\n", PORT_SMTP);

    state.smtp_socket_fd = socket_desc;

    return;

}


void *smtp_session(void *param)
{
    int client_socket = *((int *)param);
    free(param);

    struct session_smtp session;
    
    char send_buffer[BUFFER_SIZE];
    char recv_buffer[BUFFER_SIZE];
    char command[5];
    int rc;
    int data_message = 0;
    int start_index = 0;

    sprintf(send_buffer, "220 \r\n");
    rc = send(client_socket, send_buffer, strlen(send_buffer), 0);
    if (rc == -1) {
        fprintf(stderr, "ERROR at trying to send the GREETING\n");
        exit (EXIT_FAILURE);
    }


    while(1) {
        memset(send_buffer, '\0', BUFFER_SIZE);
        memset(command, '\0', 5);
        memset(recv_buffer, '\0', BUFFER_SIZE);

        rc = recv(client_socket,  recv_buffer, BUFFER_SIZE, 0);


        if (rc == 0) {
            fprintf(stderr, "Host closed the socket!\n");
            break;
        }
        if (rc == -1) {
            fprintf(stderr, "Error on sockett!\n");
            exit (EXIT_FAILURE);
        }

        fprintf(stderr, "%s", recv_buffer);

        if (data_message == 0) {
            memcpy(command, recv_buffer, 4);
            if (strcmp(command, "EHLO") == 0 || strcmp(command, "HELO") == 0) {
                strcpy(send_buffer, "250 OK\r\n");
                send(client_socket, send_buffer, strlen(send_buffer), 0);
            } else if (strcmp(command, "MAIL") == 0) {
                
                char *start = strchr(recv_buffer, '<');
                char *end = strchr(recv_buffer, '>');

                if (start && end && end > start) {
                    size_t length = end - start - 1;
                    strncpy(session.sender, start + 1, length);
                    session.sender[length] = '\0';

                    strcpy(send_buffer, "250 OK\r\n");
                    send(client_socket, send_buffer, strlen(send_buffer), 0);
                } else {
                    strcpy(send_buffer, "500 Command not recognized\r\n");
                    send(client_socket, send_buffer, strlen(send_buffer), 0);
                }
            } else if (strcmp(command, "RCPT") == 0) {
                char *start = strchr(recv_buffer, '<');
                char *end = strchr(recv_buffer, '>');

                if (start && end && end > start) {
                    size_t length = end - start - 1;
                    strncpy(session.recipient, start + 1, length);
                    session.recipient[length] = '\0';

                    strcpy(send_buffer, "250 OK\r\n");
                    send(client_socket, send_buffer, strlen(send_buffer), 0);
                } else {
                    strcpy(send_buffer, "500 Command not recognized\r\n");
                    send(client_socket, send_buffer, strlen(send_buffer), 0);
                }

            } else if (strcmp(command, "DATA") == 0) {
                strcpy(send_buffer, "354 OK\r\n");
                send(client_socket, send_buffer, strlen(send_buffer), 0);
                data_message = 1;
            } else if (strcmp(command, "QUIT") == 0) {
                strcpy(send_buffer, "221 BYE\r\n");
                send(client_socket, send_buffer, strlen(send_buffer), 0);

                MYSQL *conn;
                MYSQL_RES *res;
                MYSQL_ROW row;

                conn = mysql_init(NULL);

                if(!mysql_real_connect(conn, mail_db.server, mail_db.user, mail_db.password,
                                        mail_db.database, 0, NULL, 0)) {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    exit (-1);
                }

                mysql_free_result(res);
	            mysql_close(conn);

                break;
            } else if (strcmp(command, "NOOP") == 0) {
                strcpy(send_buffer, "250 OK\r\n");
                send(client_socket, send_buffer, strlen(send_buffer), 0);
            } else {
                strcpy(send_buffer, "500 Command not recognized\r\n");
                send(client_socket, send_buffer, strlen(send_buffer), 0);
            }
        } else if (data_message == 1) {
            memcpy(command, recv_buffer, 1);
            
            if (strcmp(command, ".") == 0) {
                strcpy(send_buffer, "250 OK\r\n");
                send(client_socket, send_buffer, strlen(send_buffer), 0);
                fprintf(stderr, "%s", session.content);
                data_message = 0;
                start_index = 0;
            } else {
                memcpy(session.content + start_index, recv_buffer, sizeof(session.content));
                start_index += rc;
            }
        }
    }

    close(client_socket);

    return ;
}


