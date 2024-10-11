#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>
#include <utility>
#include "IFileHandler.h"

class FileHandler : public IFileHandler {
public:
    FileHandler(const std::string& masterPasswordFile, const std::string& accountsFile);

    std::string loadMasterPassword() override;
    void saveMasterPassword(const std::string& encryptedMasterPassword) override;

    std::vector<std::pair<std::string, std::string>> loadAccounts() override;
    void saveAccount(const std::string& username, const std::string& encryptedPassword) override;
    void saveUpdatedAccounts(const std::vector<std::pair<std::string, std::string>>& accounts) override; // Implement this method
private:
    std::string masterPasswordFile;
    std::string accountsFile;
};

#endif // FILEHANDLER_H
