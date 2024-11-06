#include "PasswordManager.h"

#include <algorithm>

#include "FileHandler.h"
#include "GenerateRandomPassword.h"
#include <iostream>
#include <tuple>


PasswordManager::PasswordManager(std::unique_ptr<ICryptography> crypto, std::unique_ptr<IFileHandler> fileHandler)
    : crypto(std::move(crypto)), fileHandler(std::move(fileHandler)) {}

bool PasswordManager::login(const std::string& masterPassword) {
    std::string encryptedMasterPassword = fileHandler->loadMasterPassword();
    return (crypto->decrypt(encryptedMasterPassword) == masterPassword);
}

void PasswordManager::initializeMasterPassword() {
    std::string storedMasterPassword = fileHandler->loadMasterPassword();
    if (storedMasterPassword.empty()) {
        std::cout << "No master password found. Please create one: ";
        std::string newMasterPassword;
        std::getline(std::cin, newMasterPassword);

        // Encrypt and save the new master password
        std::string encryptedMasterPassword = crypto->encrypt(newMasterPassword);
        fileHandler->saveMasterPassword(encryptedMasterPassword);
        std::cout << "Master password created and saved successfully." << std::endl;
    }
}

void PasswordManager::addAccount(const std::string& accountName, const std::string& username, const std::string& password) {
    std::string encryptedPassword = crypto->encrypt(password);
    fileHandler->saveAccount(accountName, username, encryptedPassword);
}


void PasswordManager::listAccounts() {
    auto accounts = fileHandler->loadAccounts();

    if (accounts.empty()) {
        std::cout << "No accounts found." << std::endl;
        return;
    }

    std::cout << "Stored Accounts:" << std::endl;
        for (const auto& account : accounts) {
            std::cout << "Account Name: " << std::get<0>(account) << ", Username: " << std::get<1>(account) << ", Password: " << crypto->decrypt(std::get<2>(account)) << std::endl;
        }
    }



bool PasswordManager::editAccount(const std::string& oldAccountName, const std::string& newAccountName, const std::string& newUsername, const std::string& newPassword) {
    auto accounts = fileHandler->loadAccounts();

    auto it = std::find_if(accounts.begin(), accounts.end(), [&](const std::tuple<std::string, std::string, std::string>& account) {
        return std::get<0>(account) == oldAccountName;
    });

    if (it != accounts.end()) {
        // Update account name, username, and password
        std::get<0>(*it) = newAccountName;
        std::get<1>(*it) = newUsername;
        std::get<2>(*it) = crypto->encrypt(newPassword);

        // Save the updated accounts back to the file
        fileHandler->saveUpdatedAccounts(accounts);
        return true;
    } else {
        std::cout << "Account with name '" << oldAccountName << "' not found." << std::endl;
        return false;
    }
}


// Generate a random password
std::string PasswordManager::generateRandomPassword(size_t length) {
    return Utils::generateRandomPassword(length); // Assuming you have this function in Utils
}
