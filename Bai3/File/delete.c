#include <stdio.h>
#include <unistd.h>

int main () {
   int ret;
   char des[1024]; // = "temp/a.txt";
   printf("Enter dir to file: \n");
    scanf("%s", des);
   execl("/bin/rm", "rm", des, NULL);
    // if execl succeeds, this code is never used
    printf ("Could not execl file /bin/rm\n");
   return(0);
}