#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

void main()
{
	int listenfd = -1;
	int connfd = -1;
	struct sockaddr_in server_addr;
	char send_buffer[1024];

	memset(send_buffer, 0, sizeof(send_buffer));
	memset(&server_addr, 0, sizeof(server_addr));

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(5000);

	bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	listen(listenfd, 10);

    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
    printf("Client connected\n");
    while(1) {
        char mess_rev[1024];
        char mess_from_server[1024];
        printf("Server: ");
        gets(mess_from_server);
        fflush(stdin);
        send(connfd , mess_from_server , 1024 , 0);
        if (strcmp(mess_from_server, "bye") == 0) break;
        read(connfd, mess_rev, 1024);
        printf("Client: %s\n", mess_rev);
        if (strcmp(mess_rev, "bye") == 0) break;
    }
    printf("Close connected\n");
    close(connfd);
	close(listenfd);
}