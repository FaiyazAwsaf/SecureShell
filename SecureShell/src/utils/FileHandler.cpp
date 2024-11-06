#include "FileHandler.h"
#include <fstream>
#include <iostream>

FileHandler::FileHandler(const std::string& masterPasswordFile, const std::string& accountsFile)
    : masterPasswordFile(masterPasswordFile), accountsFile(accountsFile) { }

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

// Updated loadAccounts to include account name
std::vector<std::tuple<std::string, std::string, std::string>> FileHandler::loadAccounts() {
    std::vector<std::tuple<std::string, std::string, std::string>> accounts;
    std::ifstream in(accountsFile);
    if (in) {
        std::string line;
        while (std::getline(in, line)) {
            size_t firstSeparator = line.find(':');
            size_t secondSeparator = line.find(':', firstSeparator + 1);

            if (firstSeparator != std::string::npos && secondSeparator != std::string::npos) {
                std::string accountName = line.substr(0, firstSeparator);
                std::string username = line.substr(firstSeparator + 1, secondSeparator - firstSeparator - 1);
                std::string password = line.substr(secondSeparator + 1);
                accounts.emplace_back(accountName, username, password);
            }
        }
    }
    return accounts;
}

// Updated saveAccount to include account name
void FileHandler::saveAccount(const std::string& accountName, const std::string& username, const std::string& encryptedPassword) {
    std::ofstream out(accountsFile, std::ios::app); // Open in append mode
    if (out) {
        out << accountName << ":" << username << ":" << encryptedPassword << std::endl;
    }
}

// Updated saveUpdatedAccounts to include account name
void FileHandler::saveUpdatedAccounts(const std::vector<std::tuple<std::string, std::string, std::string>>& accounts) {
    std::ofstream out(accountsFile, std::ios::trunc); // Open in truncate mode to overwrite
    if (out) {
        for (const auto& account : accounts) {
            out << std::get<0>(account) << ":" << std::get<1>(account) << ":" << std::get<2>(account) << std::endl;
        }
    } else {
        std::cerr << "Error saving updated accounts!" << std::endl;
    }
}
