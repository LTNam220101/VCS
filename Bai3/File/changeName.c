#include <stdio.h>

int main () {
   int ret;
   char oldname[1024];// = "temp/a.txt";
   char newname[1024];// = "newfile.txt";
   printf("Enter dir to file: \n");
    scanf("%s", &oldname);
   printf("Enter des: \n");
    scanf("%s", &newname);
   ret = rename(oldname, newname);
	
   if(ret == 0) {
      printf("File renamed successfully");
   } else {
      printf("Error: unable to rename the file");
   }
   return(0);
}