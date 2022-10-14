#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

void main() {
    char buf[1024];
    printf("Write something to the file: \n");
    gets(buf);
    int filefd;
    printf("Choose option: \n1. Overwrite existing file\n2. Append existing file\n");
    int mode;
    scanf("%d", &mode);
    switch (mode)
    {
    case 1:
        filefd = open("write.txt",
            O_WRONLY | O_CREAT | O_TRUNC,
            S_IRUSR | S_IWUSR);
        break;
    case 2:
        filefd = open("write.txt",
            O_WRONLY | O_CREAT | O_APPEND,
            S_IRUSR | S_IWUSR);
        break;
    default:
        printf("Wrong input\n");
        return;
    }
    fflush(stdin);
    if (filefd < 0) {
        perror("Open/Create failed");
    }else {
        write(filefd, buf, strlen(buf));
        close(filefd);
    }
}