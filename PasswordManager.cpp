#include <iostream>
#include <fstream>
#include <string>
#include "SimpleEncryptLib.h"

using namespace std;

void addAccount() {
    string website, username, password;
    int passwordOption;
sss
    cout << "Enter website: ";
    cin >> website;
    cout << "Enter username: ";
    cin >> username;

    cout << "Do you want to (1) Enter your own password or (2) Generate a random password? ";
    cin >> passwordOption;

    if (passwordOption == 1) {
        cout << "Enter password: ";
        cin >> password;
    } else if (passwordOption == 2) {
        int passwordLength;
        cout << "Enter desired password length: ";
        cin >> passwordLength;

        password = SimpleEncryptLib::generateRandomPassword(passwordLength);
        cout << "Generated password: " << password << endl;
    } else {
        cout << "Invalid option. Please try again." << endl;
        return;
    }

    string encryptedPassword = SimpleEncryptLib::encrypt(password);

    ofstream outfile("passwords.txt", ios::app);
    outfile << website << " " << username << " " << encryptedPassword << endl;
    outfile.close();

    cout << "Account added successfully!" << endl;
    
}

void viewAccounts() {
    ifstream infile("passwords.txt");
    if (!infile.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    string website, username, encryptedPassword;
    cout << "Stored Accounts:\n";
    while (infile >> website >> username) {
        getline(infile, encryptedPassword);  
        if (encryptedPassword.empty()) continue;
        string decryptedPassword = SimpleEncryptLib::decrypt(encryptedPassword);
        cout << "Website: " << website << ", Username: " << username << ", Password: " << decryptedPassword << endl;
    }

    infile.close();
}

void showMenu() {
    int choice;
    do {
        cout << "\nPassword Manager Menu:\n";
        cout << "1. Add Account\n";
        cout << "2. View Accounts\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addAccount();
            break;
        case 2:
            viewAccounts();
            break;
        case 3:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 3);
}

int main() {
    showMenu();
    return 0;
}