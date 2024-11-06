#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H

#include <string>
#include <memory>
#include "ICryptography.h"
#include "IFileHandler.h"

class PasswordManager {
public:
    PasswordManager(std::unique_ptr<ICryptography> crypto, std::unique_ptr<IFileHandler> fileHandler);

    bool login(const std::string& masterPassword);
    void addAccount(const std::string& username, const std::string& password);
    void listAccounts();
    bool editAccount(const std::string& oldAccountName, const std::string& newAccountName, const std::string& newUsername, const std::string& newPassword);
    std::string generateRandomPassword(size_t length);

    // New method to initialize the master password
    void initializeMasterPassword();

    void addAccount(const std::string & string, const std::string & username, const std::string & password);

private:
    std::unique_ptr<ICryptography> crypto;
    std::unique_ptr<IFileHandler> fileHandler;
};

#endif // PASSWORDMANAGER_H
