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
#include<pthread.h>

char mess_rev[1024];
char mess_from_server[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void * socketThread(void *arg)
{
	int connfd = *((int *)arg);
  	while(1) {
		// get server's full command
		pthread_mutex_lock(&lock);  
		do{
			printf("Server: ");
			gets(mess_from_server);
			fflush(stdin);
		}while (strcmp(mess_from_server, "") == 0);
		send(connfd , mess_from_server , 1024 , 0);

		// get file's extension
		int index = strcspn(mess_from_server, ".");
		char *extension = mess_from_server + index;

		// get command
		char * token = strtok(mess_from_server, " ");
		pthread_mutex_unlock(&lock);

		// GET FILE
		if (strcmp(token, "get") == 0) {
			// create and open file with the same extension as in command
			char *name = "getCommand";
			char fileName[sizeof(name) + sizeof(extension)];
			strcpy (fileName, name) ;
			strcat (fileName, extension) ;
			int filefd = open(fileName,
			O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR);

			bool isSuccess = 1;
			ssize_t read_return;
			pthread_mutex_lock(&lock);  
			while (1) {
				read_return = read(connfd, mess_rev, 1024);
				// read err
				if (read_return == -1) {
					isSuccess = 0;
					perror("read");
					break;
				}
				// end of file
				else if (read_return == 0) {
					break;
				}
				// 
				else if (read_return < 1024) {
					write(filefd, mess_rev, read_return);
					break;
				}
				if (strcmp(mess_rev, "Read error") == 0) {
					isSuccess = 0;
					printf("Read error\n");
					break;
				}
				else if (write(filefd, mess_rev, 1024) == -1) {
					isSuccess = 0;
					perror("write");
					break;
				}
			}
			pthread_mutex_unlock(&lock);
			isSuccess ? printf("Get file completed\n") : printf("Get file failed\n");
			if (!isSuccess) remove(fileName);
			close(filefd);
		}
		//SENT FILE
		else if (strcmp(token, "sent") == 0) {
			// open file
			char * file_path = strtok(NULL, " ");
			int filefd = open(file_path, O_RDONLY);

			// open success
			if (filefd > 0){
				bool isSuccess = 1;
				ssize_t read_return;
				pthread_mutex_lock(&lock);  
				while (1) {
					read_return = read(filefd, mess_from_server, 1024);
					// read err
					if (read_return == -1) {
						perror("read");
						isSuccess = 0;
						break;
					}
					// eof
					if (read_return == 0) {
						break;
					}
					//
					else if (read_return < 1024) {
						send(connfd, mess_from_server, read_return, 0);
						break;
					}
					if (send(connfd, mess_from_server, 1024, 0) == -1) {
						perror("write");
						isSuccess = 0;
						break;
					}
				}
				pthread_mutex_unlock(&lock);
				isSuccess ? printf("Sent file completed\n") : printf("Sent file failed\n");
				close(filefd);
			}
			// can't open
			else {
				send(connfd, "Read error", 1024, 0);
			}
		}
		//CREATE PROCESS
		else if(strcmp(token, "fork") == 0){
			read(connfd, mess_rev, 1024);
			printf("%s\n", mess_rev);
		}
		//DISCONNECT
		else if(strcmp(token, "disconnect") == 0){
			printf("disconnected\n");
			send(connfd, "disconnect", 1024, 0);
			// break the while loop, close connect
			break;
		}
		else {
			// other command
			ssize_t read_return;
			while(1) {
				read_return = read(connfd, mess_rev, 1024);
				if (read_return == -1) {
					perror("read");
					break;
				}
				if (read_return == 0) {
					mess_rev[read_return] = '\0';
					break;
				}
				else if (read_return < 1024) {
					mess_rev[read_return] = '\0';
					printf("%s\n", mess_rev);
					break;
				}
				else {
					printf("%s\n", mess_rev);
					if (strcmp(mess_rev, "Read error") == 0 || strcmp(mess_rev, "") == 0)
						break;
				}
			}
		}
	}
	close(connfd);
	pthread_exit(NULL);
}

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

    pthread_t tid[10];
    int i = 0;

	listen(listenfd, 10);

	while(1){
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		printf("\nClient %d connected\n", i);
		if( pthread_create(&tid[i++], NULL, &socketThread, &connfd) != 0 )
           printf("Failed to create thread\n");
		if( i >= 10){
			i = 0;
			while(i < 10){
				pthread_join(tid[i++],NULL);
			}
			i = 0;
        }
	}
	close(listenfd);
}
