#include <iostream>
#include <string>

using namespace std;

void swapByPointer(int *xp, int *yp){
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void swapByRef(int &xp, int &yp){
    int temp = xp;
    xp = yp;
    yp = temp;
}
 
int main()
{
    int *p;
    int a = 3, b = 5;
    int c = 10, d = 30;
    p = &a;
    cout << *p << endl;
    swapByPointer(&a, &b);
    swapByRef(c, d);
    cout << a << " " << b << " " << c << " " << d << endl;
}