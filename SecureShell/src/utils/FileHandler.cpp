#include "FileHandler.h"
#include <fstream>
#include <iostream>

FileHandler::FileHandler(const std::string& masterPasswordFile, const std::string& accountsFile)
    : masterPasswordFile(masterPasswordFile), accountsFile(accountsFile) {}

std::string FileHandler::loadMasterPassword() {
    std::ifstream in(masterPasswordFile);
    if (!in) {
        return "";
    }
    std::string encryptedMasterPassword;
    std::getline(in, encryptedMasterPassword);
    return encryptedMasterPassword;
}

void FileHandler::saveMasterPassword(const std::string& encryptedMasterPassword) {
    std::ofstream out(masterPasswordFile);
    if (out) {
        out << encryptedMasterPassword << std::endl;
    }
}

std::vector<std::pair<std::string, std::string>> FileHandler::loadAccounts() {
    std::vector<std::pair<std::string, std::string>> accounts;
    std::ifstream in(accountsFile);
    if (in) {
        std::string line;
        while (std::getline(in, line)) {
            size_t separator = line.find(':');
            if (separator != std::string::npos) {
                std::string username = line.substr(0, separator);
                std::string password = line.substr(separator + 1);
                accounts.emplace_back(username, password);
            }
        }
    }
    return accounts;
}

void FileHandler::saveAccount(const std::string& username, const std::string& encryptedPassword) {
    std::ofstream out(accountsFile, std::ios::app); // Open in append mode
    if (out) {
        out << username << ":" << encryptedPassword << std::endl;
    }
}

void FileHandler::saveUpdatedAccounts(const std::vector<std::pair<std::string, std::string>>& accounts) {
    std::ofstream out(accountsFile, std::ios::trunc); // Open in truncate mode to overwrite
    if (out) {
        for (const auto& account : accounts) {
            out << account.first << ":" << account.second << std::endl;
        }
    } else {
        std::cerr << "Error saving updated accounts!" << std::endl;
    }
}
