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
	int sockfd = -1;
	struct sockaddr_in server_addr;
	char recv_buffer[1024];

	memset(recv_buffer, 0, sizeof(recv_buffer));
	memset(&server_addr, 0, sizeof(server_addr));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(5000);

	bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0) {
		printf("Server replied\n");
        while(1){
            char mess_rev[1024];
            char mess_from_client[1024];
            read(sockfd, mess_rev, 1024);
            if (strcmp(mess_rev, "bye") == 0) break;
            printf("Server: %s\nClient: ", mess_rev);
            gets(mess_from_client);
            fflush(stdin);
            send(sockfd , mess_from_client , 1024 , 0);
            if (strcmp(mess_from_client, "bye") == 0) break;
        }
        printf("Close connected\n");
	}
	close(sockfd);
}