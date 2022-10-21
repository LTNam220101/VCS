#include <bits/stdc++.h>
#include <iostream>
using namespace std;

string sum(int a, int b) {
    string result = to_string(a+b);
    return result;
}

string sum2(int a, int b) {
    if (a > INT_MAX - b){
        string str1 = to_string(a);
        string str2 = to_string(b);
        if (str1.length() > str2.length())
            swap(str1, str2);
            
        string str = "";
    
        // Calculate length of both string
        int n1 = str1.length(), n2 = str2.length();
    
        // Reverse both of strings
        reverse(str1.begin(), str1.end());
        reverse(str2.begin(), str2.end());
    
        int carry = 0;
        for (int i=0; i<n1; i++)
        {
            // Do school mathematics, compute sum of
            // current digits and carry
            int sum = ((str1[i]-'0')+(str2[i]-'0')+carry);
            str.push_back(sum%10 + '0');
    
            // Calculate carry for next step
            carry = sum/10;
        }
    
        // Add remaining digits of larger number
        for (int i=n1; i<n2; i++)
        {
            int sum = ((str2[i]-'0')+carry);
            str.push_back(sum%10 + '0');
            carry = sum/10;
        }
    
        // Add remaining carry
        if (carry)
            str.push_back(carry+'0');
    
        // reverse resultant string
        reverse(str.begin(), str.end());
        return str;
    }
    else {
        string result = to_string(a+b);
        return result;
    }
}

int main() {
    int x = INT_MAX;
    int y = INT_MAX;
    cout << sum(x, y) << endl;
    cout << sum2(x, y);
    return 0;
}