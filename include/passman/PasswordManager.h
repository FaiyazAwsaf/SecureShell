#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "passman/PasswordCrypto.h"
#include "passman/PasswordStorage.h"
#include "passman/PasswordTypes.h"

namespace passman {

class PasswordManager {
public:
    // Using PasswordEntry from PasswordTypes.h

    /**
     * @brief Constructor for PasswordManager
     * @param dataDir Directory where password files will be stored
     */
    explicit PasswordManager(const std::string& dataDir = "data/");
    ~PasswordManager() = default;

    bool initialize(const std::string& masterPassword);
    bool authenticate(const std::string& masterPassword) const;
    bool changeMasterPassword(const std::string& oldPassword, const std::string& newPassword);
    bool addEntry(const std::string& service, const std::string& username, const std::string& password);
    bool removeEntry(const std::string& service);
    bool updateEntry(const std::string& service, const std::string& username, const std::string& password);
    std::vector<std::string> listServices() const;
    PasswordEntry getEntry(const std::string& service) const;
    std::string getPassword(const std::string& service) const; // New method to get decrypted password
    std::string generatePassword(size_t length = 16) const;
    
    /**
     * @brief Checks if a master password has been set up
     * @return True if a master password exists, false otherwise
     */
    bool hasMasterPassword() const;

    /**
     * @brief Loads the password data from storage
     * @return True if loading was successful, false otherwise
     */
    bool load();

private:
    /**
     * @brief Verifies if the input password matches the master password
     * @param inputPassword The password to verify
     * @return True if the password matches, false otherwise
     */
    bool verifyMasterPassword(const std::string& inputPassword) const;
    
    /**
     * @brief Converts a service name to lowercase for case-insensitive lookup
     * @param service The service name to convert
     * @return The lowercase version of the service name
     */
    std::string getServiceKey(const std::string& service) const;
    
    /**
     * @brief Loads passwords from storage
     * @return True if loading was successful, false otherwise
     */
    bool loadPasswords();
    
    /**
     * @brief Saves passwords to storage
     * @return True if saving was successful, false otherwise
     */
    bool savePasswords() const;

    std::string masterPasswordHash;
    std::string masterSalt;
    std::unordered_map<std::string, PasswordEntry> passwords;
    PasswordCrypto crypto;
    PasswordStorage storage;
};

} // namespace passman