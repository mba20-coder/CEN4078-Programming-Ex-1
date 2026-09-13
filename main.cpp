/**
CEN 4078 Promgramming Exercise: 1
File Name:  almazan-programming-1.cpp

The programming exerise 1 is for testing login functionality with secure software requirements
and testing the input validation oand type checking

@author Mark Almazan
@version 1.0*/

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cctype>
#include <conio.h>

using namespace std;

struct Account {
    string username;
    string password;
    int mfaToken;
};

class Validator {
public:
    bool sqlInjection(const string& input) {
        string invalidChars = "/-;\"";
        for (char c : input) {
            if (invalidChars.find(c) != string::npos) {
                return false;
            }
        }
        return true;
    }

    bool passwordPolicy(const string& password) {
        if (password.length() < 8 || password.length() > 12) {
            return false;
        }

        bool hasUpper = false;
        bool hasLower = false;
        bool hasDigit = false;

        for (char c : password) {
            if (isupper(static_cast<unsigned char>(c))) {
                hasUpper = true;
            }
            if (islower(static_cast<unsigned char>(c))) {
                hasLower = true;
            }
            if (isdigit(static_cast<unsigned char>(c))) {
                hasDigit = true;
            }
        }

        return hasUpper && hasLower && hasDigit;
    }

    bool integerOverflow(const string& input) {
        if (input.empty()) {
            return false;
        }

        try {
            long long value = stoll(input);
            return value >= -2147483648LL && value <= 2147483647LL;
        } catch (...) {
            return false;
        }
    }
};

const Account accounts[] = {
    {"scientist", "OrgoCh101", 1234567890},
    {"engineer", "MechE202", 1357924680},
    {"security", "Cyber303", 1122334455}
};

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
    int mfaToken = 0;
    char ch;
    Validator validator;

    writeToFile(accounts, sizeof(accounts) / sizeof(accounts[0]), "accounts.txt");

    cout << "Welcome to Login Central - Login on demand!" << endl;
    cout << "Enter username: ";
    cin >> username;

    if (!validator.sqlInjection(username)) {
        cout << "Login failed. Please try again." << endl;
        return 0;
    }

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

    if (!validator.sqlInjection(password) || !validator.passwordPolicy(password)) {
        cout << "Login failed. Please try again." << endl;
        return 0;
    }

    bool validLogin = false;
    const Account* matchedAccount = nullptr;
    for (const Account& account : accounts) {
        if (username == account.username && password == account.password) {
            matchedAccount = &account;
            break;
        }
    }

    if (matchedAccount != nullptr) {
        cout << "Enter MFA token (10-digit number): ";
        cin >> mfaTokenInput;

        if (!validator.integerOverflow(mfaTokenInput) || mfaTokenInput.length() != 10) {
            cout << "Login failed. Please try again." << endl;
            return 0;
        }

        try {
            mfaToken = stoi(mfaTokenInput);
        } catch (...) {
            cout << "Login failed. Please try again." << endl;
            return 0;
        }

        if (mfaToken == matchedAccount->mfaToken) {
            cout << "Login successful - Welcome " << username << "!" << endl;
        } else {
            cout << "Login failed. Please try again." << endl;
        }
    } else {
        cout << "Login failed. Please try again." << endl;
    }

    return 0;
}
