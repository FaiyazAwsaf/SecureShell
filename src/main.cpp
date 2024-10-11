#include "password_manager/PasswordManager.h"
#include "cryptography_library/CaesarCipher.h"
#include "cryptography_library/XORCipher.h"
#include <iostream>
#include <ctime>
#include <memory> // for std::unique_ptr

int main() {
    srand(static_cast<unsigned>(time(0))); // Initialize random seed

    std::string masterPassword;
    std::cout << "Set master password for your password manager: ";
    std::getline(std::cin, masterPassword);

    // Use CaesarCipher for encryption (can easily swap this with XORCipher or any other)
    std::unique_ptr<ICryptography> crypto = std::make_unique<CaesarCipher>(3);
    PasswordManager passwordManager(std::move(crypto), masterPassword);

    std::string inputPassword;
    std::cout << "Enter master password to login: ";
    std::getline(std::cin, inputPassword);

    if (!passwordManager.login(inputPassword)) {
        std::cout << "Incorrect master password. Exiting..." << std::endl;
        return 1;
    }

    std::cout << "Login successful!" << std::endl;

    bool running = true;
    while (running) {
        std::cout << "\n1. Add Account\n2. List Accounts\n3. Exit\nChoose an option: ";
        int option;
        std::cin >> option;
        std::cin.ignore();  // Ignore newline after input

        switch (option) {
            case 1: {
                std::string username, password;
                std::cout << "Enter username: ";
                std::getline(std::cin, username);

                std::cout << "Do you want to generate a random password? (yes/no): ";
                std::string choice;
                std::getline(std::cin, choice);

                if (choice == "yes") {
                    password = passwordManager.generateRandomPassword(12);
                    std::cout << "Generated password: " << password << std::endl;
                } else {
                    std::cout << "Enter password: ";
                    std::getline(std::cin, password);
                }

                passwordManager.addAccount(username, password);
                std::cout << "Account added!" << std::endl;
                break;
            }
            case 2:
                passwordManager.listAccounts();
                break;
            case 3:
                running = false;
                break;
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
        }
    }

    return 0;
}
