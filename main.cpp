#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>

#include "Account.h"
#include "Validator.h"

using namespace std;

void writeToFile(const Account accounts[], size_t count, const string& fileName) {
    ofstream outFile(fileName);
    if (!outFile) {
        cerr << "Could not open " << fileName << " for writing." << endl;
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        outFile << accounts[i].username << " " << accounts[i].password << endl;
    }

    outFile.close();
    cout << "Account list saved to " << fileName << endl;
}

int main() {
    string username;
    string password;
    string mfaTokenInput;
    char ch;
    Validator validator;

    writeToFile(accounts, accountCount, "accounts.txt");

    cout << "Welcome to Login Central - Login on demand!" << endl;
    cout << "Enter username: ";
    cin >> username;

    if (!validator.sqlInjectionCheck(username)) {
        cout << "Login failed. Please check your credentials and try again." << endl;
        return 0;
    }

    cout << "Enter password: ";
    while (true) {
        ch = _getch();
        if (ch == 13) {
            break;
        } else if (ch == 8) {
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

    if (!validator.sqlInjectionCheck(password) || !validator.passwordPolicyCheck(password)) {
        cout << "Login failed. Please check your credentials and try again." << endl;
        return 0;
    }

    bool validLogin = false;
    for (size_t i = 0; i < accountCount; ++i) {
        if (username == accounts[i].username && password == accounts[i].password) {
            validLogin = true;
            break;
        }
    }

    if (validLogin) {
        cout << "Enter MFA token (10-digit number): ";
        cin >> mfaTokenInput;

        if (!validator.integerOverflowCheck(mfaTokenInput) || mfaTokenInput.length() != 10) {
            cout << "Login failed. Please check your credentials and try again." << endl;
            return 0;
        }

        cout << "Login successful - Welcome " << username << "!" << endl;
    } else {
        cout << "Login failed. Please check your credentials and try again." << endl;
    }

    return 0;
}
