#include<stdio.h>
#include <string.h>
 
char* func(char *str, int n) {
    int i;
    char result[n];
    for ( i = 0; i < n; i++ ) {
        result[i] = str[i];
    }
    return result;
}

int main() { 
    char *p = "Helloooo world!";
    char *res = func(p, 20);
    printf("%s", res);
    return 0;
}