#include <iostream>
#include <string>

using namespace std;

class Student {
    private:
        static const bool def_isActive = true;
    protected:
        std::string name;
        int age;
        float avgPoint;
        bool isActive;
    public:
        Student(std::string name, int age, float point, bool isActive = def_isActive) : name(name), age(age), avgPoint(point), isActive(isActive) {}

        std::string getInfo(){
            return name + " " + to_string(age) + " " + to_string(avgPoint) + " " + (isActive ? "active" : "not active");
        }
};


string helloStr(string str) {
    return "Hello " + str;
}

int main()
{
   string name;
   int age;
   float point;
   bool isActive;
   cout << "Nhap ten: " << endl;
   cin >> name;
   cout << "Nhap tuoi: " << endl;
   cin >> age;
   cout << "Nhap diem TB: " << endl;
   cin >> point;
   Student a = Student(name, age, point);
   cout << "Nhap ten: " << endl;
   cin >> name;
   cout << "Nhap tuoi: " << endl;
   cin >> age;
   cout << "Nhap diem TB: " << endl;
   cin >> point;
   cout << "HS con hoc? (co: 1, khong: 0) " << endl;
   cin >> isActive;
   Student b = Student(name, age, point, isActive);
   cout << a.getInfo() << endl;
   cout << b.getInfo() << endl;
   return(0);
}