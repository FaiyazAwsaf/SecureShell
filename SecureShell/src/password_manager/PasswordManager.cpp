#include "PasswordManager.h"
#include "FileHandler.h"
#include "GenerateRandomPassword.h"
#include <iostream>

// Constructor implementation
PasswordManager::PasswordManager(std::unique_ptr<ICryptography> crypto, std::unique_ptr<IFileHandler> fileHandler)
    : crypto(std::move(crypto)), fileHandler(std::move(fileHandler)) {}

// Login method
bool PasswordManager::login(const std::string& masterPassword) {
    // Check against the stored master password
    std::string encryptedMasterPassword = fileHandler->loadMasterPassword();
    return (crypto->decrypt(encryptedMasterPassword) == masterPassword);
}

// Add an account
void PasswordManager::addAccount(const std::string& username, const std::string& password) {
    std::string encryptedPassword = crypto->encrypt(password);
    fileHandler->saveAccount(username, encryptedPassword);
}

// List all accounts
void PasswordManager::listAccounts() {
    auto accounts = fileHandler->loadAccounts();

    if (accounts.empty()) {
        std::cout << "No accounts found." << std::endl;
        return;
    }

    std::cout << "Stored Accounts:" << std::endl;
    for (const auto& account : accounts) {
        std::cout << "Username: " << account.first << ", Password: " << crypto->decrypt(account.second) << std::endl;
    }
}

// Generate a random password
std::string PasswordManager::generateRandomPassword(size_t length) {
    return Utils::generateRandomPassword(length); // Assuming you have this function in Utils
}
