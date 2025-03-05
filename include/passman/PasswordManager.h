#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class PasswordManager {
public:
    struct PasswordEntry {
        std::string service;
        std::string username;
        std::string passwordHash;
        std::string serviceLink;
        std::string salt;
    };

    PasswordManager();
    ~PasswordManager() = default;

    bool initialize(const std::string& masterPassword);
    bool authenticate(const std::string& masterPassword) const;
    bool changeMasterPassword(const std::string& oldPassword, const std::string& newPassword);
    bool addEntry(const std::string& service, const std::string& username, const std::string& password);
    bool removeEntry(const std::string& service);
    bool updateEntry(const std::string& service, const std::string& username, const std::string& password);
    std::vector<std::string> listServices() const;
    PasswordEntry getEntry(const std::string& service) const;
    std::string generatePassword(size_t length = 16) const;

private:
    std::string customHash(const std::string& input, const std::string& salt) const;
    std::string generateSalt(size_t length = 16) const;
    bool loadPasswords();
    bool savePasswords() const;
    bool verifyMasterPassword(const std::string& inputPassword) const;

    std::string masterPasswordHash;
    std::string masterSalt;
    std::unordered_map<std::string, PasswordEntry> passwords;
    const std::string passwordFile = "passwords.dat";
};