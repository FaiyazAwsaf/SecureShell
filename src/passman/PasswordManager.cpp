#include "passman/PasswordManager.h"
#include <algorithm>
#include <sstream>
#include <filesystem>
#include <fstream>

namespace passman {

PasswordManager::PasswordManager(const std::string& dataDir)
    : storage(dataDir) {
}

bool PasswordManager::initialize(const std::string& masterPassword) {
    masterSalt = crypto.generateSalt();
    masterPasswordHash = crypto.customHash(masterPassword, masterSalt);
    return loadPasswords();
}

bool PasswordManager::authenticate(const std::string& masterPassword) const {
    return verifyMasterPassword(masterPassword);
}

bool PasswordManager::verifyMasterPassword(const std::string& inputPassword) const {
    return crypto.customHash(inputPassword, masterSalt) == masterPasswordHash;
}

bool PasswordManager::changeMasterPassword(const std::string& oldPassword, const std::string& newPassword) {
    if (!verifyMasterPassword(oldPassword) || newPassword.empty()) {
        return false;
    }

    std::string newSalt = crypto.generateSalt();
    std::string newHash = crypto.customHash(newPassword, newSalt);

    std::string oldHash = masterPasswordHash;
    std::string oldSalt = masterSalt;

    masterPasswordHash = newHash;
    masterSalt = newSalt;

    if (!savePasswords()) {
        masterPasswordHash = oldHash;
        masterSalt = oldSalt;
        return false;
    }

    return true;
}

bool PasswordManager::addEntry(const std::string& service, const std::string& username, const std::string& password) {
    std::string salt = crypto.generateSalt();
    PasswordEntry entry{
        service,
        username,
        crypto.encryptPassword(password, masterPasswordHash), // Use encryption with master password hash as key
        "", // serviceLink is empty by default
        salt
    };

    passwords[service] = entry;
    return savePasswords();
}

bool PasswordManager::removeEntry(const std::string& service) {
    for (auto it = passwords.begin(); it != passwords.end(); ++it) {
        if (getServiceKey(it->first) == getServiceKey(service)) {
            passwords.erase(it);
            return savePasswords();
        }
    }
    return false;
}

    // Directly modifying entry instead of calling addEntry
bool PasswordManager::updateEntry(const std::string& service, const std::string& username, const std::string& password) {
    for (auto it = passwords.begin(); it != passwords.end(); ++it) {
        if (getServiceKey(it->first) == getServiceKey(service)) {
            std::string originalService = it->first;
            return addEntry(originalService, username, password);
        }
    }
    return false;
}

std::vector<std::string> PasswordManager::listServices() const {
    std::vector<std::string> services;
    services.reserve(passwords.size());
    for (const auto& pair : passwords) {
        services.push_back(pair.first);
    }
    return services;
}

PasswordEntry PasswordManager::getEntry(const std::string& service) const {
    for (const auto& pair : passwords) {
        if (getServiceKey(pair.first) == getServiceKey(service)) {
            return pair.second;
        }
    }
    return PasswordEntry{};
}

std::string PasswordManager::generatePassword(size_t length) const {
    return crypto.generatePassword(length);
}

bool PasswordManager::loadPasswords() {
    if (storage.loadMasterPassword(masterPasswordHash, masterSalt)) {
        return storage.loadPasswords(passwords, masterPasswordHash);
    }
    return true; // New file is not an error
}

bool PasswordManager::savePasswords() const {
    if (!storage.saveMasterPassword(masterPasswordHash, masterSalt)) {
        return false;
    }
    return storage.savePasswords(passwords, masterPasswordHash);
}

std::string PasswordManager::getPassword(const std::string& service) const {
    for (const auto& pair : passwords) {
        if (getServiceKey(pair.first) == getServiceKey(service)) {
            return crypto.decryptPassword(pair.second.encryptedPassword, masterPasswordHash);
        }
    }
    return "";
}

std::string PasswordManager::getServiceKey(const std::string& service) const {
    std::string key = service;
    std::transform(key.begin(), key.end(), key.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return key;
}

bool PasswordManager::hasMasterPassword() const {
    std::string tempHash, tempSalt;
    return storage.loadMasterPassword(tempHash, tempSalt);
}

bool PasswordManager::load() {
    return loadPasswords();
}

} // namespace passman