#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>
#include <tuple>
#include "IFileHandler.h"

class FileHandler : public IFileHandler {
public:
    // Constructor with file paths for master password and accounts storage
    FileHandler(const std::string& masterPasswordFilePath, const std::string& accountsFilePath);

    // Methods to load and save the master password
    std::string loadMasterPassword() override;
    void saveMasterPassword(const std::string& encryptedMasterPassword) override;

    // Methods to load and save account information
    std::vector<std::tuple<std::string, std::string, std::string>> loadAccounts() override;
    void saveAccount(const std::string& accountName, const std::string& username, const std::string& encryptedPassword) override;
    void saveUpdatedAccounts(const std::vector<std::tuple<std::string, std::string, std::string>>& accounts) override;

private:
    // File paths for storing master password and account data
    std::string masterPasswordFile;
    std::string accountsFile;
};

#endif // FILEHANDLER_H
