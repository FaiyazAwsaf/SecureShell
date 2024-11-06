#ifndef IFILEHANDLER_H
#define IFILEHANDLER_H

#include <string>
#include <vector>
#include <tuple>

class IFileHandler {
public:
    virtual ~IFileHandler() = default;

    virtual void saveMasterPassword(const std::string& encryptedPassword) = 0;
    virtual std::string loadMasterPassword() = 0;

    // Updated method to save an account with account name, username, and encrypted password
    virtual void saveAccount(const std::string& accountName, const std::string& username, const std::string& encryptedPassword) = 0;

    // Updated to return a tuple of account name, username, and encrypted password
    virtual std::vector<std::tuple<std::string, std::string, std::string>> loadAccounts() = 0;

    // Updated to accept tuples with account name, username, and encrypted password
    virtual void saveUpdatedAccounts(const std::vector<std::tuple<std::string, std::string, std::string>>& accounts) = 0;
};

#endif // IFILEHANDLER_H
