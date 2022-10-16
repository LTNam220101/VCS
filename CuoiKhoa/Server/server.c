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

void main()
{
	int listenfd = -1;
	int connfd = -1;
	struct sockaddr_in server_addr;

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
		char * token = strtok(mess_from_server, " ");
		// GET FILE
		if (strcmp(token, "get") == 0) {
			int filefd = open("getFromClient.txt",
			O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR);
			bool isSuccess = 1;
			ssize_t read_return;
			while (1) {
				read_return = read(connfd, mess_rev, 1024);
				if (read_return == -1) {
					isSuccess = 0;
					perror("read");
					break;
				}
				else if (read_return == 0) {
					break;
				}
				else if (read_return < 1024) {
					write(filefd, mess_rev, read_return);
					break;
				}
				if (write(filefd, mess_rev, 1024) == -1) {
					isSuccess = 0;
					perror("write");
					break;
				}
			}
			isSuccess ? printf("Get file completed\n") : printf("Get file failed\n");
			close(filefd);
		}
		//SENT FILE
		else if (strcmp(token, "sent") == 0) {
			char * file_path = strtok(NULL, " ");
			int filefd = open(file_path, O_RDONLY);
			ssize_t read_return;
			while (1) {
				read_return = read(filefd, mess_from_server, 1024);
				if (read_return == -1) {
					perror("read");
                    break;
				}
				if (read_return == 0) {
					break;
				}
				else if (read_return < 1024) {
					send(connfd, mess_from_server, read_return, 0);
					break;
				}
				if (send(connfd, mess_from_server, 1024, 0) == -1) {
					perror("write");
                    break;
				}
			}
			printf("Sent file completed\n");
			close(filefd);
		}
		//DISCONNECT
		else if(strcmp(token, "disconnect") == 0){
			printf("disconnected\n");
			send(connfd, "disconnect", 1024, 0);
			break;
		}
		else {
			ssize_t read_return;
			read_return = read(connfd, mess_rev, 1024);
			if (read_return == -1) {
				perror("read");
			}
			else {
				printf("%s\n", mess_rev);
			}
		}
	}
	close(connfd);
	close(listenfd);
}
