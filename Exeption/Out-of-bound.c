#include<stdio.h>
#include <string.h>
#include <ctype.h>

void func(char p[]) {
    int i;
    for(i=0; i<=strlen(p); i++){
        *(p+i) -= 32;
    }
    printf("%s", p);
}

void func2(char p[]) {
    int i;
    for(i=0; i<=strlen(p); i++){
        if (*(p+i) == '\0') break;
        *(p+i) -= 32;
    }
    printf("%s", p);
}

int main() { 
    char p[] = "helloworld";
    func2(p);
    func(p);
    return 0;
}