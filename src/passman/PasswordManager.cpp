#include "passman/PasswordManager.h"
#include <algorithm>
#include <sstream>
#include <filesystem>
#include <fstream>

namespace passman {

PasswordManager::PasswordManager(const std::string& dataDir)
    : storage(dataDir) {
    // Initialize with empty values
    // The actual values will be loaded when initialize() or loadPasswords() is called
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

    // Save current state in case of failure
    std::string oldHash = masterPasswordHash;
    std::string oldSalt = masterSalt;

    masterPasswordHash = newHash;
    masterSalt = newSalt;

    if (!savePasswords()) {
        // Restore old state if save fails
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

    // Store with original case but use lowercase key for lookup
    passwords[service] = entry;
    return savePasswords();
}

bool PasswordManager::removeEntry(const std::string& service) {
    // Case-insensitive lookup
    for (auto it = passwords.begin(); it != passwords.end(); ++it) {
        if (getServiceKey(it->first) == getServiceKey(service)) {
            passwords.erase(it);
            return savePasswords();
        }
    }
    return false;
}

bool PasswordManager::updateEntry(const std::string& service, const std::string& username, const std::string& password) {
    // Case-insensitive lookup
    for (auto it = passwords.begin(); it != passwords.end(); ++it) {
        if (getServiceKey(it->first) == getServiceKey(service)) {
            // Keep the original service name case but update other fields
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
    // Case-insensitive lookup
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
    // First load master password information
    if (storage.loadMasterPassword(masterPasswordHash, masterSalt)) {
        // Then load password entries
        return storage.loadPasswords(passwords, masterPasswordHash);
    }
    return true; // New file is not an error
}

bool PasswordManager::savePasswords() const {
    // First save master password information
    if (!storage.saveMasterPassword(masterPasswordHash, masterSalt)) {
        return false;
    }
    // Then save password entries
    return storage.savePasswords(passwords, masterPasswordHash);
}

std::string PasswordManager::getPassword(const std::string& service) const {
    // Find the service using case-insensitive lookup
    for (const auto& pair : passwords) {
        if (getServiceKey(pair.first) == getServiceKey(service)) {
            return crypto.decryptPassword(pair.second.encryptedPassword, masterPasswordHash);
        }
    }
    return "";
}

std::string PasswordManager::getServiceKey(const std::string& service) const {
    // Convert service name to lowercase for case-insensitive comparison
    std::string key = service;
    std::transform(key.begin(), key.end(), key.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return key;
}

bool PasswordManager::hasMasterPassword() const {
    // Check if master password file exists by trying to load it
    std::string tempHash, tempSalt;
    return storage.loadMasterPassword(tempHash, tempSalt);
}

} // namespace passman