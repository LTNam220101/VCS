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
#include<sys/wait.h>

void main()
{
	int sockfd = -1;
	struct sockaddr_in server_addr;

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
			char * token = strtok(mess_rev, " ");
			//SERVER GET
			if(strcmp(token, "get") == 0){
				char * file_path = strtok(NULL, " ");
				printf("%s", file_path);
                int filefd = open(file_path, O_RDONLY);
				if (filefd > 0) {
					bool isSuccess = 1;
					ssize_t read_file;
					while (1) {
						read_file = read(filefd, mess_from_client, 1024);
						printf("%s", mess_from_client);
						if (read_file == -1) {
							perror("read");
							isSuccess = 0;
							break;
						}
						if (send(sockfd, mess_from_client, 1024, 0) == -1) {
							perror("write");
							isSuccess = 0;
							break;
						}
					}
					isSuccess ? send(sockfd, "ok", 1024, 0) : send(sockfd, "fail", 1024, 0);
					isSuccess ? printf("Sent file completed\n") : printf("Sent file failed\n");
					close(filefd);
				}else {
					send(sockfd, "fail", 1024, 0);
				}
            }
			//SERVER SENT
			else if(strcmp(token, "sent") == 0){
                int filefd = open("getFromServer.txt",
                O_WRONLY | O_CREAT | O_TRUNC,
                S_IRUSR | S_IWUSR);
				ssize_t read_return;
				while (1) {
					read_return = read(sockfd, mess_rev, 1024);
					printf("%s ", mess_rev);
					if (strcmp(mess_rev, "ok") == 0) {
						break;
					}
					if (read_return == -1) {
						perror("read");
                        break;
					}
					if (write(filefd, mess_rev, 1024) == -1) {
						perror("write");
                        break;
					}
				}
            	printf("Get file completed\n");
				close(filefd);
            }
			//DISCONNECT
			else if(strcmp(token, "disconnect") == 0){
				printf("disconnected\n");
				break;
			}
			else {
				pid_t child_pid = fork();
				if (child_pid != 0)
				{
					wait(NULL);
					//open the text to get output
					int filefd = open("Client/message.txt", O_RDONLY);
					ssize_t read_file;
					read_file = read(filefd, mess_from_client, 1024);
					printf("\n%s\n", mess_from_client);
					if (read_file == -1) {
						perror("read");
					}
					if (send(sockfd, mess_from_client, 1024, 0) == -1) {
						perror("write");
					}
					printf("Completed\n");
					close(filefd);
				} else {
					int i;
					char *argv[10];
					for ( i = 0; i < 8; i++){
						argv[i] = strtok(NULL, " ");
					}
					// execute command, save the output to message.txt
					execl("Client/scr.sh", "Client/scr.sh", token, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], NULL);
				}
			}
        }
	close(sockfd);
	}
}