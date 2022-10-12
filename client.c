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
			char * token = strtok(mess_rev, " ");
			if (strcmp(token, "pwd") == 0) {
				send(sockfd, getenv("PWD"), 1024, 0);
            	printf("Get pwd completed\n");
			}
			else if (strcmp(token, "ls") == 0) {
				pid_t child_pid = fork();
				if (child_pid != 0)
				{
					pid_t status;
					wait(&status);
					printf("%d", status);
					if (status == 512){
						send(sockfd, "fail", 1024, 0);
					}else {
						int filefd = open("abc.txt", O_RDONLY);
						ssize_t read_file;
						while (1) {
							read_file = read(filefd, mess_from_client, 1024);
							if (read_file == 0)
								break;
							if (read_file == -1) {
								perror("read");
								exit(EXIT_FAILURE);
							}
							/* TODO use write loop: https://stackoverflow.com/questions/24259640/writing-a-full-buffer-using-write-system-call */
							if (send(sockfd, mess_from_client, 1024, 0) == -1) {
								perror("write");
								exit(EXIT_FAILURE);
							}
						}
						// send(sockfd, "ok", 1024, 0);
						printf("get ls completed\n");
						close(filefd);
					}
				}else {
					char * argv = strtok(NULL, " ");
					execl("./scr.sh", "./scr.sh", argv, NULL);
				}
			}
			else if (strcmp(token, "touch") == 0) {
				pid_t child_pid = fork();
				if (child_pid != 0) 
				{
					wait();
            		printf("touch completed\n");
				}else {
					char * argv = strtok(NULL, " ");
					execl("/bin/touch", "touch", argv, NULL);
				}
				send(sockfd, getenv("PWD"), 1024, 0);
			}
			else if(strcmp(token, "get") == 0){
				char * file_path = strtok(NULL, " ");
                int filefd = open(file_path, O_RDONLY);
				if (filefd > 0) {
					bool isSuccess = 1;
					ssize_t read_file;
					while (1) {
						read_file = read(filefd, mess_from_client, 1024);
						if (read_file == 0)
							isSuccess = 0;
							break;
						if (read_file == -1) {
							perror("read");
							isSuccess = 0;
							break;
						}
						/* TODO use write loop: https://stackoverflow.com/questions/24259640/writing-a-full-buffer-using-write-system-call */
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
			else if(strcmp(token, "sent") == 0){
                int filefd = open("getFromServer.txt",
                O_WRONLY | O_CREAT | O_TRUNC,
                S_IRUSR | S_IWUSR);
				ssize_t read_return;
				while (1) {
					read_return = read(sockfd, mess_rev, 1024);
					if (read_return == 0) {
						break;
					}
					if (strcmp(mess_rev, "ok") == 0) {
						break;
					}
					if (read_return == -1) {
						perror("read");
						exit(EXIT_FAILURE);
					}
					if (write(filefd, mess_rev, 1024) == -1) {
						perror("write");
						exit(EXIT_FAILURE);
					}
				}
            	printf("Get file completed\n");
				close(filefd);
            }
			else if(strcmp(token, "kill") == 0){
                pid_t child_pid = fork();
				if (child_pid != 0) 
				{
					printf("%d", child_pid);
					uint32_t tmp = htonl(child_pid);
					send(sockfd, &tmp, sizeof(tmp), 0);
					// /* this code is only executed in the child process */ 
					// printf ("I am a child and my pid = %d\n", getpid());
					// execl("/bin/ls", "ls", "-l", NULL);
					// /* if execl succeeds, this code is never used */
					// printf ("Could not execl file /bin/ls\n");
					// /* this exit stops only the child process */        
					// exit(1);
	            	printf("kill process completed\n");
				}else {
					// while(1) {
					// 	printf("123");
					// }
				}
				
            }
			else {
            	printf("Not supported\n");
			}
            // gets(mess_from_client);
            // fflush(stdin);
            // send(sockfd , mess_from_client , 1024 , 0);
        }
	close(sockfd);
	}
}