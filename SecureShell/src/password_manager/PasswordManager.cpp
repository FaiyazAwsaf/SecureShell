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

bool PasswordManager::editAccount(const std::string& oldUsername, const std::string& newUsername, const std::string& newPassword) {
    auto accounts = fileHandler->loadAccounts();
    bool accountFound = false;

    for (auto& account : accounts) {
        if (account.first == oldUsername) {
            // Update username and password
            account.first = newUsername;
            account.second = crypto->encrypt(newPassword);
            accountFound = true;
            break;
        }
    }

    if (accountFound) {
        // Save the updated accounts back to the file
        fileHandler->saveUpdatedAccounts(accounts);
        return true;
    } else {
        std::cout << "Account with username '" << oldUsername << "' not found." << std::endl;
        return false;
    }
}

// Generate a random password
std::string PasswordManager::generateRandomPassword(size_t length) {
    return Utils::generateRandomPassword(length); // Assuming you have this function in Utils
}
