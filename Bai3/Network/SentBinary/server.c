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

    connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);

    /* Open the file that we wish to transfer */
    int fd = open("index.jpeg", O_RDONLY);
    if(fd < 0)
    {
        printf("File opern error");
        return 1;   
    }   

    /* Read data from file and send it */
    while(1)
    {
        /* First read file in chunks of 256 bytes */
        unsigned char buff[256]={0};
        int nread = read(fd,buff,256);
        printf("Bytes read %d \n", nread);        

        /* If read was success, send data. */
        if(nread > 0)
        {
            printf("Sending \n");
            write(connfd, buff, nread);
        }

        /*
            * There is something tricky going on with read .. 
            * Either there was error, or we reached end of file.
            */
        if (nread < 256)
        {
            printf("End of file\n");
            break;
        }
    }
    close(connfd);
    close(listenfd);


    return 0;
}