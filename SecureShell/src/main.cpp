#include "PasswordManager.h"
#include "CaesarCipher.h"
#include "FileHandler.h"
#include <iostream>
#include <ctime>
#include <memory>

int main() {
    srand(static_cast<unsigned>(time(0))); // Initialize random seed

    std::unique_ptr<ICryptography> crypto = std::make_unique<CaesarCipher>(3);
    std::unique_ptr<IFileHandler> fileHandler = std::make_unique<FileHandler>("D:/IUT UG/3rd Sem/Labs/SPL 1/SecureShell/Data/master_password.txt", "D:/IUT UG/3rd Sem/Labs/SPL 1/SecureShell/Data/accounts.txt");

    PasswordManager passwordManager(std::move(crypto), std::move(fileHandler));

    // Initialize or check for master password
    passwordManager.initializeMasterPassword();

    std::string inputPassword;
    std::cout << "Enter master password to login: ";
    std::getline(std::cin, inputPassword);

    if (!passwordManager.login(inputPassword)) {
        std::cout << "Incorrect master password. Exiting..." << std::endl;
        return 1;
    }

    std::cout << "Login successful!" << std::endl;

    // Main loop for managing accounts
    bool running = true;
    while (running) {
        std::cout << "\n1. Add Account\n2. List Accounts\n3. Edit Account\n4. Exit\nChoose an option: ";
        int option;
        std::cin >> option;
        std::cin.ignore();  // Ignore newline after input

        switch (option) {
            case 1: {
                std::string accountName, username, password;

                // Prompt for account name (e.g., website or application name)
                std::cout << "Enter the name of the website: ";
                std::getline(std::cin, accountName);

                std::cout << "Enter username: ";
                std::getline(std::cin, username);

                std::cout << "Do you want to generate a random password? (yes / no): ";
                std::string choice;
                std::getline(std::cin, choice);

                if (choice == "yes") {
                    password = passwordManager.generateRandomPassword(12);
                    std::cout << "Generated password: " << password << std::endl;
                } else {
                    std::cout << "Enter password: ";
                    std::getline(std::cin, password);
                }

                // Add account with website/account name, username, and password
                passwordManager.addAccount(accountName, username, password);
                std::cout << "Account added!" << std::endl;
                break;
            }
            case 2:
                passwordManager.listAccounts();
                break;
            case 3: {
                std::string oldAccountName, newAccountName, newUsername, newPassword;

                std::cout << "Enter the name of the website/account to edit: ";
                std::getline(std::cin, oldAccountName);
                std::cout << "Enter new account name: ";
                std::getline(std::cin, newAccountName);
                std::cout << "Enter new username: ";
                std::getline(std::cin, newUsername);
                std::cout << "Enter new password: ";
                std::getline(std::cin, newPassword);

                if (passwordManager.editAccount(oldAccountName, newAccountName, newUsername, newPassword)) {
                    std::cout << "Account updated successfully!" << std::endl;
                } else {
                    std::cout << "Account not found." << std::endl;
                }
                break;
            }
            case 4:
                running = false;
                break;
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
        }
    }

    return 0;
}
