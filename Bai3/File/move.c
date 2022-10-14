#include <stdio.h>
#include <unistd.h>

int main () {
   int ret;
   char des[1024], newDes[1024]; // = "temp/a.txt";
   printf("Enter dir to file: \n");
    scanf("%s", des);
   printf("Enter new dir: \n");
    scanf("%s", newDes);
   execl("/bin/mv", "mv", des, newDes, NULL);
    // if execl succeeds, this code is never used
    printf ("Could not execl file /bin/mv\n");
   return(0);
}