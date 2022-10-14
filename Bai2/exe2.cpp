#include <iostream>
#include <string>

using namespace std;
int main()
{
   string str = "Hello ";
   char str2[30];
   cout << "Nhap ten: " << endl;
   cin >> str2;
   cout << str << str2 << endl;
   return(0);
}