#pragma once

#include <string>
#include <unordered_map>
#include "encryption/FileEncryption.h"
#include "passman/PasswordTypes.h"

namespace passman {

// No need for forward declaration of PasswordManager anymore

/**
 * @class PasswordStorage
 * @brief Handles file I/O operations for password management
 * 
 * This class is responsible for loading and saving password data to disk,
 * including encryption and decryption of the password file.
 */
class PasswordStorage {
public:
    /**
     * @brief Constructor that takes data directory path
     * @param dataDir Directory where password files will be stored
     */
    explicit PasswordStorage(const std::string& dataDir = "data/");
    ~PasswordStorage() = default;

    /**
     * @brief Loads master password information from disk
     * @param masterPasswordHash Reference to store the loaded master password hash
     * @param masterSalt Reference to store the loaded master salt
     * @return True if loading was successful, false otherwise
     */
    bool loadMasterPassword(std::string& masterPasswordHash, std::string& masterSalt) const;
    
    /**
     * @brief Saves master password information to disk
     * @param masterPasswordHash The master password hash to save
     * @param masterSalt The master salt to save
     * @return True if saving was successful, false otherwise
     */
    bool saveMasterPassword(const std::string& masterPasswordHash, const std::string& masterSalt) const;
    
    /**
     * @brief Loads password entries from disk
     * @param passwords Reference to store the loaded password entries
     * @param masterPasswordHash The master password hash for decryption
     * @return True if loading was successful, false otherwise
     */
    bool loadPasswords(std::unordered_map<std::string, PasswordEntry>& passwords, 
                      const std::string& masterPasswordHash) const;
    
    /**
     * @brief Saves password entries to disk
     * @param passwords The password entries to save
     * @param masterPasswordHash The master password hash for encryption
     * @return True if saving was successful, false otherwise
     */
    bool savePasswords(const std::unordered_map<std::string, PasswordEntry>& passwords, 
                      const std::string& masterPasswordHash) const;

private:
    const std::string dataDir;
    const std::string passwordFile;
    const std::string masterFile;
    FileEncryption encryptor;
};

} // namespace passman