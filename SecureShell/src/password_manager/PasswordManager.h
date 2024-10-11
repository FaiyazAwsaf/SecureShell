#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include "ICryptography.h"
#include "IFileHandler.h"

struct Account {
    std::string username;
    std::string password; // stored encrypted
};

class PasswordManager {
public:
    PasswordManager(std::unique_ptr<ICryptography> crypto, std::unique_ptr<IFileHandler> fileHandler);

    bool login(const std::string& masterPassword);
    void addAccount(const std::string& username, const std::string& password);
    void listAccounts();
    std::string generateRandomPassword(size_t length);
    bool editAccount(const std::string& oldUsername, const std::string& newUsername, const std::string& newPassword); // New function to edit accounts

private:
    std::unique_ptr<ICryptography> crypto;
    std::unique_ptr<IFileHandler> fileHandler;
};

#endif // PASSWORD_MANAGER_H
