#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    printf("%d", getpid());
    while(1)
    {
        printf("Chuan bi sleep trong mot vai giay ...\n");
        sleep(1);
    }
    return(0);
}
