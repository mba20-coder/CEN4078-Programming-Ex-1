/**
CEN 4078 Programming Exercise: 2
File Name:  almazan-programming-2.cpp
Date: 10/04/2026

Programming exercise 2 is focused on storing usernames and passwords as a Vigenere chipertext.
This program allows for two attempst to log in, and the credentials must follow a specified password policy.

@author Mark Almazan
@version 2.0*/

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cctype>
#include <conio.h>
#include <stdexcept>
#include <random>
#include <algorithm>

using namespace std;

const string ALPHA_KEY = "ARGOSROCK";
const string NUMBER_KEY = "1963";
const int max_attempts = 2;

struct Account
{
    string username;
    string password;
    int mfaToken;
};

struct PasswordPolicy
{
    size_t minLength = 8;
    size_t maxLength = 12;
    bool requireUpper = true;
    bool requireLower = true;
    bool requireDigit = true;
    bool alphanumericOnly = true;
};

// created cryptographer class for Vigenere cipher encryption/decryption
// V2.0 10/04/2026
class cryptographer
{
private:
    string alphaKey;
    string numberKey;

    char shiftLetter(char c, char keyChar, int direction)
    {
        char base = isupper(static_cast<unsigned char>(c)) ? 'A' : 'a';
        int shift = toupper(static_cast<unsigned char>(keyChar)) - 'A';
        int position = c - base;
        int newPosition = (position + direction * shift + 26) % 26;
        return static_cast<char>(base + newPosition);
    }

    char shiftDigit(char c, char keyChar, int direction)
    {
        int shift = keyChar - '0';
        int position = c - '0';
        int newPosition = (position - direction * shift + 10) % 10;
        return static_cast<char>('0' + newPosition);
    }

    string vigenereLetters(const string &key, const string &text, int direction)
    {
        if (key.empty())
        {
            return text;
        }

        string result = text;
        size_t keyIndex = 0;

        for (char &c : result)
        {
            if (isalpha(static_cast<unsigned char>(c)))
            {
                char keyChar = key[keyIndex % key.length()];
                c = shiftLetter(c, keyChar, direction);
                ++keyIndex;
            }
        }
        return result;
    }

    string vigenereNumbers(const string &key, const string &text, int direction)
    {
        if (key.empty())
        {
            return text;
        }

        string result = text;
        size_t keyIndex = 0;

        for (char &c : result)
        {
            if (isdigit(static_cast<unsigned char>(c)))
            {
                char keyChar = key[keyIndex % key.length()];
                c = shiftDigit(c, keyChar, direction);
                ++keyIndex;
            }
        }
        return result;
    }

public:
    cryptographer(const string &alpha = ALPHA_KEY, const string &number = NUMBER_KEY)
        : alphaKey(alpha), numberKey(number) {}

    string encryptVigenere(const string &alphaKey, const string &clearText)
    {
        return vigenereLetters(alphaKey, clearText, 1);
    }

    string decryptVigenere(const string &alphaKey, const string &cipherText)
    {
        return vigenereLetters(alphaKey, cipherText, -1);
    }

    string encryptNumber(const string &numberKey, const string &clearText)
    {
        return vigenereNumbers(numberKey, clearText, 1);
    }

    string decryptNumber(const string &numberKey, const string &cipherText)
    {
        return vigenereNumbers(numberKey, cipherText, -1);
    }

    string encrypt(const string &clearText)
    {
        return encryptNumber(numberKey, encryptVigenere(alphaKey, clearText));
    }

    string decrypt(const string &cipherText)
    {
        return decryptNumber(numberKey, decryptVigenere(alphaKey, cipherText));
    }
};

class Validator
{
public:
    bool sqlInjection(const string &input)
    {
        string invalidChars = "/-;\"";
        for (char c : input)
        {
            if (invalidChars.find(c) != string::npos)
            {
                return false;
            }
        }
        return true;
    }

    bool isAlphanumeric(const string &input)
    {
        if (input.empty())
        {
            return false;
        }

        for (char c : input)
        {
            if (!isalnum(static_cast<unsigned char>(c)))
            {
                return false;
            }
        }
        return true;
    }

    // updated password policy to better suit secure software development practices
    // V2.0 10/04/2026
    PasswordPolicy getPasswordPolicy() const
    {
        PasswordPolicy policy;
        return policy;
    }

    string promptPasswordPolicy() const
    {
        PasswordPolicy policy = getPasswordPolicy();
        return "Password must be " + to_string(policy.minLength) + "-" +
               to_string(policy.maxLength) + " characters long. It must contain only letters and numbers. "
                                             "With at least one Uppercase and Lowercase letter, and one number.";
    }

    bool passwordPolicy(const string &password)
    {
        PasswordPolicy policy = getPasswordPolicy();

        if (password.length() < policy.minLength || password.length() > policy.maxLength)
        {
            return false;
        }

        if (policy.alphanumericOnly && !isAlphanumeric(password))
        {
            return false;
        }

        bool hasUpper = false;
        bool hasLower = false;
        bool hasDigit = false;

        for (char c : password)
        {
            if (isupper(static_cast<unsigned char>(c)))
                hasUpper = true;
            if (islower(static_cast<unsigned char>(c)))
                hasLower = true;
            if (isdigit(static_cast<unsigned char>(c)))
                hasDigit = true;
        }

        if (policy.requireUpper && !hasUpper)
            return false;
        if (policy.requireLower && !hasLower)
            return false;
        if (policy.requireDigit && !hasDigit)
            return false;

        return true;
    }

    bool integerOverflow(const string &input)
    {
        if (input.empty())
        {
            return false;
        }

        try
        {
            long long value = stoll(input);
            return value >= -2147483648LL && value <= 2147483647LL;
        }
        catch (...)
        {
            return false;
        }
    }
};

// added default password class to create policy compliant password
// V2.0 10/04/2026
class defaultPassword
{
private:
    PasswordPolicy policy;
    mt19937 rng;

    char randomFrom(const string &chars)
    {
        uniform_int_distribution<size_t> dist(0, chars.length() - 1);
        return chars[dist(rng)];
    }

public:
    defaultPassword(const PasswordPolicy &p) : policy(p), rng(random_device{}()) {}

    string generate()
    {
        const string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const string lower = "abcdefghijklmnopqrstuvwxyz";
        const string digits = "0123456789";
        const string all = upper + lower + digits;

        uniform_int_distribution<size_t> lengthPick(policy.minLength, policy.maxLength);
        size_t length = lengthPick(rng);

        string password;
        password += randomFrom(upper);
        password += randomFrom(lower);
        password += randomFrom(digits);

        while (password.length() < length)
        {
            password += randomFrom(all);
        }

        shuffle(password.begin(), password.end(), rng);
        return password;
    }

    void notifyUser()
    {
        cout << "Your password is set to a default password." << endl;
        cout << "You will receive a secure email with your password." << endl;
    }

    string setDefaultPassword(Validator &validator)
    {
        string password = generate();
        while (!validator.passwordPolicy(password))
        {
            password = generate();
        }
        notifyUser();
        return password;
    }
};

// changed the default accounts to the cipher text
// V2.0 10/04/2026
const Account accounts[] = {
    {"stosfkwud", "OimcUy015", 1234567890},
    {"eemwfvst", "MvivW116", 1357924680},
    {"sviijzha", "Cphsj217", 1122334455}};

void writeToFile(const Account accounts[], size_t count, const string &fileName)
{
    ofstream outFile(fileName);
    if (!outFile)
    {
        cerr << "Could not open " << fileName << " for writing." << endl;
        return;
    }

    for (size_t i = 0; i < count; ++i)
    {
        outFile << accounts[i].username << " " << accounts[i].password << endl;
    }

    outFile.close();
}

void loginFailed()
{
    cout << "Login failed. Please try again." << endl;
}

string readPassword()
{
    string password;
    char ch;

    cout << "Enter Password: ";
    while (true)
    {
        ch = _getch();
        if (ch == 13)
        {
            break;
        }
        else if (ch == 8)
        {
            if (!password.empty())
            {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else
        {
            password += ch;
            cout << '*';
        }
    }
    cout << endl;
    return password;
}

int main()
{
    string username;
    string password;
    string mfaTokenInput;
    int mfaToken = 0;
    Validator validator;
    cryptographer crypto;

    writeToFile(accounts, sizeof(accounts) / sizeof(accounts[0]), "accounts.txt");

    cout << "Welcome to Login Central - Login on demand!" << endl;
    cout << "Enter username: ";
    cin >> username;

    // Added max password attempts = 2
    // V2.0 10/04/2026
    bool validPassword = false;
    for (int attempt = 1; attempt <= max_attempts; ++attempt)
    {
        password = readPassword();

        if (validator.sqlInjection(password) && validator.passwordPolicy(password))
        {
            validPassword = true;
            break;
        }

        if (attempt < max_attempts)
        {
            cout << "Password does not meet Password Policy." << endl;
            cout << validator.promptPasswordPolicy() << endl;
        }
    }

    if (!validPassword)
    {
        defaultPassword resetter(validator.getPasswordPolicy());
        string newPassword = resetter.setDefaultPassword(validator);
        newPassword.assign(newPassword.length(), '\0');
        return 0;
    }

    if (!validator.sqlInjection(username) || !validator.isAlphanumeric(username))
    {
        loginFailed();
        return 0;
    }

    string encryptedUsername = crypto.encrypt(username);
    string encryptedPassword = crypto.encrypt(password);

    const Account *matchedAccount = nullptr;
    for (const Account &account : accounts)
    {
        bool encryptedMatch = (encryptedUsername == account.username &&
                               encryptedPassword == account.password);

        bool decryptedMatch = (crypto.decrypt(account.username) == username &&
                               crypto.decrypt(account.password) == password);

        if (encryptedMatch && decryptedMatch)
        {
            matchedAccount = &account;
            break;
        }
    }

    if (matchedAccount != nullptr)
    {
        cout << "Enter MFA token (10-digit number): ";
        cin >> mfaTokenInput;

        if (!validator.integerOverflow(mfaTokenInput) || mfaTokenInput.length() != 10)
        {
            loginFailed();
            return 0;
        }

        try
        {
            mfaToken = stoi(mfaTokenInput);
        }
        catch (...)
        {
            loginFailed();
            return 0;
        }

        if (mfaToken == matchedAccount->mfaToken)
        {
            cout << "Login successful - Welcome " << username << "!" << endl;
        }
        else
        {
            loginFailed();
        }
    }
    else
    {
        loginFailed();
    }

    return 0;
}
