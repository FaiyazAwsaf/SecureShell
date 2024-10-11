#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "IFileHandler.h"
#include <fstream>
#include <string>
#include <vector>

class FileHandler : public IFileHandler {
public:
    FileHandler(const std::string& masterPasswordFile, const std::string& accountsFile);

    void saveMasterPassword(const std::string& encryptedPassword) override;
    std::string loadMasterPassword() override;
    void saveAccount(const std::string& username, const std::string& encryptedPassword) override; // Ensure this matches
    std::vector<std::pair<std::string, std::string>> loadAccounts() override;

private:
    std::string masterPasswordFile;
    std::string accountsFile;
};

#endif // FILE_HANDLER_H
