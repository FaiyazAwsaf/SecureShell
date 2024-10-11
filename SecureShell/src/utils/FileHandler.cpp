#include "FileHandler.h"
#include <iostream>

// Constructor
FileHandler::FileHandler(const std::string& masterPasswordFile, const std::string& accountsFile)
    : masterPasswordFile(masterPasswordFile), accountsFile(accountsFile) {}

// Save master password to file
void FileHandler::saveMasterPassword(const std::string& encryptedPassword) {
    std::ofstream out(masterPasswordFile);
    if (out) {
        out << encryptedPassword;
    } else {
        std::cerr << "Error saving master password!" << std::endl;
    }
}

// Load master password from file
std::string FileHandler::loadMasterPassword() {
    std::string encryptedPassword;
    std::ifstream in(masterPasswordFile);
    if (in) {
        std::getline(in, encryptedPassword);
    }
    return encryptedPassword;
}

// Save account information
void FileHandler::saveAccount(const std::string& username, const std::string& encryptedPassword) {
    std::ofstream out(accountsFile, std::ios::app); // Open file in append mode
    if (out) {
        out << username << ":" << encryptedPassword << std::endl; // Store in format "username:encryptedPassword"
    } else {
        std::cerr << "Error saving account!" << std::endl;
    }
}

// Load accounts from file
std::vector<std::pair<std::string, std::string>> FileHandler::loadAccounts() {
    std::vector<std::pair<std::string, std::string>> accounts;
    std::ifstream in(accountsFile);
    std::string line;

    while (std::getline(in, line)) {
        size_t delimiterPos = line.find(':');
        if (delimiterPos != std::string::npos) {
            std::string username = line.substr(0, delimiterPos);
            std::string encryptedPassword = line.substr(delimiterPos + 1);
            accounts.emplace_back(username, encryptedPassword);
        }
    }

    return accounts;
}
