#include <string.h>
#include <iostream>

using namespace std;

void copy(char* string) {
    char a[4];
    strcpy(a, string);
    cout << a;
}

void copy2(char* string) {
    int len = strlen(string);
    char* a;
    a = new char[len];
    strcpy(a, string);
    cout << a;
}

int main() {
    copy2("a string longer than 4 characters");
    return 0;
}