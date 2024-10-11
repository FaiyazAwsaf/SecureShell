// PasswordManager.cpp
#include "PasswordManager.h"
#include <iostream>
#include <cstdlib>  // for rand()
#include <ctime>    // for time()

PasswordManager::PasswordManager(std::unique_ptr<ICryptography> crypto, const std::string &masterPassword)
    : cryptography(std::move(crypto)) {
    masterPasswordHash = cryptography->hash(masterPassword); // Hashing master password
}

// Login by hashing the entered password and comparing with the stored hash
bool PasswordManager::login(const std::string &password) {
    return masterPasswordHash == cryptography->hash(password);
}

// Add an account (password is encrypted before storage)
void PasswordManager::addAccount(const std::string &username, const std::string &password) {
    std::string encryptedPassword = cryptography->encrypt(password);
    accounts.push_back({username, encryptedPassword});
}

// List all accounts (decrypt passwords before displaying)
void PasswordManager::listAccounts() const {
    for (const auto &account : accounts) {
        std::string decryptedPassword = cryptography->decrypt(account.password);
        std::cout << "Username: " << account.username << ", Password: " << decryptedPassword << std::endl;
    }
}

// Generate a random password
std::string PasswordManager::generateRandomPassword(int length) {
    std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string password;
    for (int i = 0; i < length; ++i) {
        password += characters[rand() % characters.length()];
    }
    return password;
}
