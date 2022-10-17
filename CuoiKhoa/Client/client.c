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
#include <signal.h>

char* convertIntegerToChar(int N) {
 
    // Count digits in number N
    int m = N;
    int digit = 0;
    while (m) {
 
        // Increment number of digits
        digit++;
 
        // Truncate the last
        // digit from the number
        m /= 10;
    }
 
    // Declare char array for result
    char* arr;
 
    // Declare duplicate char array
    char arr1[digit];
 
    // Memory allocation of array
    arr = (char*)malloc(digit);
 
    // Separating integer into digits and
    // accommodate it to character array
    int index = 0;
    while (N) {
 
        // Separate last digit from
        // the number and add ASCII
        // value of character '0' is 48
        arr1[++index] = N % 10 + '0';
 
        // Truncate the last
        // digit from the number
        N /= 10;
    }
 
    // Reverse the array for result
    int i;
    for (i = 0; i < index; i++) {
        arr[i] = arr1[index - i];
    }
 
    // Char array truncate by null
    arr[i] = '\0';
 
    // Return char array
    return (char*)arr;
}

void sighandler(int signum) {
	printf("Bat duoc tin hieu %d, chuan bi thoat ...\n", signum);
   	exit(1);
}

void main() {
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
			// get server's full command
            read(sockfd, mess_rev, 1024);

			// get file's extension
			int index = strcspn(mess_rev, ".");
			char *extension = mess_rev + index;

			// get command
			char * token = strtok(mess_rev, " ");

			//SERVER GET
			if(strcmp(token, "get") == 0){
				// open file
				char * file_path = strtok(NULL, " ");
                int filefd = open(file_path, O_RDONLY);

				// open success
				if (filefd > 0) {
					bool isSuccess = 1;
					ssize_t read_file;
					while (1) {
						read_file = read(filefd, mess_from_client, 1024);
						// read err
						if (read_file == -1) {
							perror("read");
							send(sockfd, "Read error", 1024, 0);
							isSuccess = 0;
							break;
						}
						// eof
						if (read_file == 0) {
							send(sockfd, mess_from_client, 1024, 0);
							break;
						}
						//
						else if (read_file < 1024) {
							send(sockfd, mess_from_client, read_file, 0);
							break;
						}
						if (send(sockfd, mess_from_client, 1024, 0) == -1) {
							perror("write");
							isSuccess = 0;
							break;
						}
					}
					isSuccess ? printf("Sent file completed\n") : printf("Sent file failed\n");
					close(filefd);
				}
				// can't open
				else {
					send(sockfd, "Read error", 1024, 0);
				}
            }
			//SERVER SENT
			else if(strcmp(token, "sent") == 0){
				// create and open file with the same extension as in command
				char *name = "sentCommand";
				char fileName[sizeof(name) + sizeof(extension)];
				strcpy (fileName, name) ;
				strcat (fileName, extension) ;
                int filefd = open(fileName,
                O_WRONLY | O_CREAT | O_TRUNC,
                S_IRUSR | S_IWUSR);

				ssize_t read_return;
				bool isSuccess = 1;
				while (1) {
					read_return = read(sockfd, mess_rev, 1024);
					// read err
					if (read_return == -1) {
						perror("read");
						isSuccess = 0;
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
						printf("Read error\n");
						isSuccess = 0;
						break;
					}
					else if (write(filefd, mess_rev, 1024) == -1) {
						perror("write");
						isSuccess = 0;
                        break;
					}
				}
            	isSuccess ? printf("Get file completed\n") : printf("Get file failed\n");
				if (!isSuccess) remove(fileName);
				close(filefd);
            }
			//CREATE PROCESS
			else if(strcmp(token, "fork") == 0){
				pid_t child_pid = fork();
				if (child_pid != 0)
				{
					char* arr = convertIntegerToChar(child_pid);
					send(sockfd, arr, 1024, 0);
					
				}else {
					execl("Client/fork", "Client/fork", NULL);
				}
			}
			//DISCONNECT
			else if(strcmp(token, "disconnect") == 0){
				printf("disconnected\n");
				// break the while loop, close connect
				break;
			}
			else {
				// other command
				pid_t child_pid = fork();
				if (child_pid != 0)
				{
					waitpid(child_pid, NULL, 0);
					//open the message.txt, send the output to server
					int filefd = open("Client/message.txt", O_RDONLY);
					ssize_t read_file;
					while (1) {
						read_file = read(filefd, mess_from_client, 1024);
						// err
						if (read_file == -1) {
							send(sockfd, "Read error", 1024, 0);
							perror("read");
							break;
						}
						// 
						if (read_file == 0) {
							mess_from_client[read_file] = '\0';
							send(sockfd, mess_from_client, 1024, 0);
							break;
						}
						//
						else if (read_file < 1024) {
							mess_from_client[read_file] = '\0';
							send(sockfd, mess_from_client, read_file, 0);
							break;
						}
						if (send(sockfd, mess_from_client, 1024, 0) == -1) {
							perror("write");
							break;
						}
					}
					printf("Completed\n");
					close(filefd);
					if (getppid() == 0) {
						return;
					}
				} else {
					// get arguments
					int i;
					char *argv[10];
					for ( i = 0; i < 8; i++){
						argv[i] = strtok(NULL, " ");
					}
					// execute command by shell script, save the output to message.txt
					execl("Client/scr.sh", "Client/scr.sh", token, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], NULL);
				}
			}
        }
	close(sockfd);
	}
}