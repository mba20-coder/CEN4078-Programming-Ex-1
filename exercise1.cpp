/**
CEN 4078 Promgramming Exercise: 1
File Name:  almazan-programming-1.cpp

The programming exerise 1 is for testing login functionality with secure software requirements
and testing the input validation oand type checking

@author Mark Almazan
@version 1.0*/

#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <conio.h>

using namespace std;

struct Account {
    string username;
    string password;
};

const Account accounts[] = {
    {"Scientist", "Orgo101"},
    {"Engineer", "MechE101"},
    {"Security", "Cyber101"}
};

int main() {
    string username;
    string password;
    char ch;

    cout << "Welcome to Login Central - Login on demand!" << endl;
    cout << "Enter username: ";
    cin >> username;

    cout << "Enter password: ";
    while (true) {
        ch = _getch();
        if (ch == 13) { // Enter key
            break;
        } else if (ch == 8) { // Backspace key
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        } else {
            password += ch;
            cout << '*';
        }
    }
    cout << endl;

    bool validLogin = false;
    for (const Account& account : accounts) {
        if (username == account.username && password == account.password) {
            validLogin = true;
            break;
        }
    }

    if (validLogin) {
        cout << "Login successful!" << endl;
    } else {
        cout << "Invalid username or password." << endl;
    }

    return 0;
}
