// PasswordManager.h
#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include "ICryptography.h"

struct Account {
    std::string username;
    std::string password; // stored encrypted
};

class PasswordManager {
private:
    std::unique_ptr<ICryptography> cryptography;
    std::string masterPasswordHash;
    std::vector<Account> accounts;

public:
    PasswordManager(std::unique_ptr<ICryptography> crypto, const std::string &masterPassword);

    bool login(const std::string &password);
    void addAccount(const std::string &username, const std::string &password);
    void listAccounts() const;
    std::string generateRandomPassword(int length);
};

#endif // PASSWORD_MANAGER_H
