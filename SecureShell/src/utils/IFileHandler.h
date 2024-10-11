#ifndef IFILEHANDLER_H
#define IFILEHANDLER_H

#include <string>
#include <vector>

class IFileHandler {
public:
    virtual ~IFileHandler() = default;

    virtual void saveMasterPassword(const std::string& encryptedPassword) = 0;
    virtual std::string loadMasterPassword() = 0;
    virtual void saveAccount(const std::string& username, const std::string& encryptedPassword) = 0; // Ensure this is declared
    virtual std::vector<std::pair<std::string, std::string>> loadAccounts() = 0;
    virtual void saveUpdatedAccounts(const std::vector<std::pair<std::string, std::string>>& accounts) = 0; // New method to save updated accounts

};

#endif // IFILEHANDLER_H
