#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

int main(void)
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

        /* Create file where data will be stored */
        int fd = open("recv.txt", O_WRONLY | O_CREAT); 
        if(fd < 0)
        {
            printf("Error opening file");
            return 1;
        }

        int bytesReceived = 0;
        char recvBuff[256];
        memset(recvBuff, '0', sizeof(recvBuff));
        /* Receive data in chunks of 256 bytes */
        while((bytesReceived = read(sockfd, recvBuff, 256)) > 0)
        {
            if (write(fd, recvBuff, 256) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        if(bytesReceived < 0)
        {
            printf("\n Read Error \n");
        }
    }

    return 0;
}