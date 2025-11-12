#include <iostream>
#include <map>
#include <string>
using namespace std;

#define MIN_BALANCE 500

// Exception class for insufficient funds
class InsufficientFunds { };

// Base Account class
class Account {
private:
    long accountNumber;
    string firstName;
    string lastName;
    float balance;
    static long NextAccountNumber;

public:
    Account() {}
    Account(string fname, string lname, float bal);
    long getAccNo() const { return accountNumber; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    float getBalance() const { return balance; }

    void Deposit(float amount);
    void Withdraw(float amount);

    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();

    friend ostream & operator << (ostream & os, const Account & acc);
};

// Initialize static variable
long Account::NextAccountNumber = 0;

// Constructor
Account::Account(string fname, string lname, float bal) {
    accountNumber = ++NextAccountNumber;
    firstName = fname;
    lastName = lname;
    balance = bal;
}

void Account::Deposit(float amount) {
    if (amount <= 0) {
        cout << "Invalid deposit amount.\n";
        return;
    }
    balance += amount;
}

void Account::Withdraw(float amount) {
    if (amount <= 0) {
        cout << "Invalid withdrawal amount.\n";
        return;
    }
    if (balance - amount < MIN_BALANCE)
        throw InsufficientFunds();
    balance -= amount;
}

void Account::setLastAccountNumber(long accNum) {
    NextAccountNumber = accNum;
}

long Account::getLastAccountNumber() {
    return NextAccountNumber;
}

ostream & operator<<(ostream & os, const Account & acc) {
    os << "\nAccount Number: " << acc.accountNumber
       << "\nFirst Name: " << acc.firstName
       << "\nLast Name: " << acc.lastName
       << "\nBalance: " << acc.balance << "\n";
    return os;
}

// Bank class managing multiple accounts
class Bank {
private:
    map<long, Account> accounts;

public:
    Account OpenAccount(string fname, string lname, float balance);
    void BalanceEnquiry(long accountNumber);
    void Deposit(long accountNumber, float amount);
    void Withdraw(long accountNumber, float amount);
    void CloseAccount(long accountNumber);
    void ShowAllAccounts();
};

// Member function definitions
Account Bank::OpenAccount(string fname, string lname, float balance) {
    if (balance < MIN_BALANCE) {
        cout << "Initial balance must be at least " << MIN_BALANCE << ".\n";
        return Account();
    }

    Account acc(fname, lname, balance);
    accounts.insert({acc.getAccNo(), acc});

    cout << "\nAccount Created Successfully!\n";
    cout << acc;
    return acc;
}

void Bank::BalanceEnquiry(long accountNumber) {
    auto it = accounts.find(accountNumber);
    if (it != accounts.end()) {
        cout << it->second;
    } else {
        cout << "Account Not Found.\n";
    }
}

void Bank::Deposit(long accountNumber, float amount) {
    auto it = accounts.find(accountNumber);
    if (it != accounts.end()) {
        it->second.Deposit(amount);
        cout << "\nAmount deposited successfully.\n";
        cout << it->second;
    } else {
        cout << "Account Not Found.\n";
    }
}

void Bank::Withdraw(long accountNumber, float amount) {
    auto it = accounts.find(accountNumber);
    if (it != accounts.end()) {
        try {
            it->second.Withdraw(amount);
            cout << "\nAmount withdrawn successfully.\n";
            cout << it->second;
        } catch (InsufficientFunds &) {
            cout << "Insufficient Balance.\n";
        }
    } else {
        cout << "Account Not Found.\n";
    }
}

void Bank::CloseAccount(long accountNumber) {
    if (accounts.erase(accountNumber))
        cout << "Account Closed Successfully.\n";
    else
        cout << "Account Not Found.\n";
}

void Bank::ShowAllAccounts() {
    if (accounts.empty()) {
        cout << "No accounts available.\n";
        return;
    }
    for (auto &acc : accounts)
        cout << acc.second << endl;
}

// Main driver code
int main() {
    Bank bank;
    int choice;
    long accNum;
    string fname, lname;
    float balance, amount;

    do {
        cout << "\n===== Banking System Menu =====\n";
        cout << "1. Open Account\n";
        cout << "2. Balance Enquiry\n";
        cout << "3. Deposit\n";
        cout << "4. Withdraw\n";
        cout << "5. Close Account\n";
        cout << "6. Show All Accounts\n";
        cout << "7. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter First Name: ";
            cin >> fname;
            cout << "Enter Last Name: ";
            cin >> lname;
            cout << "Enter Initial Balance: ";
            cin >> balance;
            bank.OpenAccount(fname, lname, balance);
            break;

        case 2:
            cout << "Enter Account Number: ";
            cin >> accNum;
            bank.BalanceEnquiry(accNum);
            break;

        case 3:
            cout << "Enter Account Number: ";
            cin >> accNum;
            cout << "Enter Deposit Amount: ";
            cin >> amount;
            bank.Deposit(accNum, amount);
            break;

        case 4:
            cout << "Enter Account Number: ";
            cin >> accNum;
            cout << "Enter Withdraw Amount: ";
            cin >> amount;
            bank.Withdraw(accNum, amount);
            break;

        case 5:
            cout << "Enter Account Number: ";
            cin >> accNum;
            bank.CloseAccount(accNum);
            break;

        case 6:
            bank.ShowAllAccounts();
            break;

        case 7:
            cout << "Thank you for using Banking System!\n";
            break;

        default:
            cout << "Invalid Option!\n";
        }
    } while (choice != 7);

    return 0;
}