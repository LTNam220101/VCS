#include <stdio.h>
#include <fcntl.h>

void main() {
    int filefd = open("create.txt",
        O_WRONLY | O_CREAT | O_TRUNC,
        S_IRUSR | S_IWUSR);
    if (filefd < 0) {
        perror("Open/Create failed");
    }else {
        printf("Open/Create successed");
    }
}