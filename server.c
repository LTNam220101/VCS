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

char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s)); 
}

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

	while(1) {
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
			if (strcmp(token, "pwd") == 0) {
				read(connfd, mess_rev, 1024);
				printf("%s\n", mess_rev);
			}
			else if (strcmp(token, "ls") == 0) {
				ssize_t read_return;
				read_return = read(connfd, mess_rev, 1024);
				if (read_return == 0) {
				}
				if (strcmp(mess_rev, "fail") == 0) {
					printf("get ls failed, wrong direction\n");
				}
				if (strcmp(mess_rev, "ok") == 0) {
				}
				if (read_return == -1) {
					perror("read");
					exit(EXIT_FAILURE);
				}
				else {
					printf("%s\n", mess_rev);
				}
			}
			else if (strcmp(token, "touch") == 0) {
				read(connfd, mess_rev, 1024);
				printf("%s\n", mess_rev);
			}
			else if (strcmp(token, "get") == 0) {
				int filefd = open("getFromClient.txt",
                O_WRONLY | O_CREAT | O_TRUNC,
                S_IRUSR | S_IWUSR);
				ssize_t read_return;
				bool isSuccess = 1;
				while (1) {
					read_return = read(connfd, mess_rev, 1024);
					if (read_return == 0) {
						break;
					}
					if (strcmp(mess_rev, "ok") == 0) {
						break;
					}
					if (strcmp(mess_rev, "fail") == 0) {
						isSuccess = 0;
						break;
					}
					if (read_return == -1) {
						isSuccess = 0;
						perror("read");
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
			else if (strcmp(token, "sent") == 0) {
				char * file_path = strtok(NULL, " ");
				int filefd = open(file_path, O_RDONLY);
				ssize_t read_file;
				while (1) {
					read_file = read(filefd, mess_from_server, 1024);
					if (read_file == 0)
						break;
					if (read_file == -1) {
						perror("read");
						exit(EXIT_FAILURE);
					}
					/* TODO use write loop: https://stackoverflow.com/questions/24259640/writing-a-full-buffer-using-write-system-call */
					if (send(connfd, mess_from_server, 1024, 0) == -1) {
						perror("write");
						exit(EXIT_FAILURE);
					}
				}
				send(connfd, "ok", 1024, 0);
            	printf("Sent file completed\n");
				close(filefd);
			}
			else if(strcmp(token, "kill") == 0){
				uint32_t tmp,n;
				read(connfd, &tmp, sizeof(tmp));
				n = ntohl(tmp);
				// read(connfd, mess_from_server, 1024);
				// int received_int = ntohl(received_int);
				printf("%d", n);
            	printf("kill process completed\n");
            }
        }
		close(connfd);
	}
	close(listenfd);
}
