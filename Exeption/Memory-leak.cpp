#include <string.h>
#include <iostream>

void memleak() {
    while(1) {
        int *p = new int;
    }
}

void memleak2() {
    while(1) {
        int *p = new int;
        delete p;
    }
}

int main() {
    memleak2();
    return 0;
}