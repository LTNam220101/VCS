#include <iostream>

using namespace std;
int main (){
    int n;
    do {
        cout << "Enter n: ";
        cin >> n;
    }while (n <= 0);
    int i;
    for ( i = 0; i < n; i++ ){
        cout << i << " ";
    }
    cout << endl;
    char c;
    cout << "Enter c: ";
    cin >> c;
    switch (c)
    {
    case 'a':
        cout << "A\n";
        break;
    default:
        cout << "Not A\n";
        break;
    }
    return 0;
}