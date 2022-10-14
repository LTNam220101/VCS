#include <iostream>
#include <string>

using namespace std;

class Account {
private:   
    static constexpr const char *def_name = "Unnamed Account";
    static constexpr double def_balance = 0.0;
protected:
    std::string name;
    double balance;
public:
    Account(std::string name = def_name, double balance = def_balance): name{name}, balance{balance} {
    };
    // Account(std::string name = "Unamed Account", double balance = 0.0);
    void setBalance(double balance){
        this->balance = balance;
    }
    bool deposit(double amount){
        if (amount < 0) 
            return false;
        else {
            balance += amount;
            return true;
        }
    };
    bool withdraw(double amount){
        if (balance-amount >=0) {
            balance-=amount;
            return true;
        } else
            return false;
    };
    double get_balance() const{
        return balance;
    };
};

int main() {
    Account *account = new Account();
    Account account2 = Account("LTN", 100.0);
    account->setBalance(200);
    cout << account->get_balance() << " " << account2.get_balance() << endl;
}