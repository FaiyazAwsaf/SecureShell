#pragma once

#include <string>
#include <vector>
#include <random>

namespace passman {

/**
 * @class PasswordCrypto
 * @brief Handles all cryptographic operations for password management
 * 
 * This class is responsible for encryption, decryption, hashing, and salt generation
 * operations used in the password management system.
 */
class PasswordCrypto {
public:
    PasswordCrypto() = default;
    ~PasswordCrypto() = default;

    /**
     * @brief Creates a custom hash of the input string with the provided salt
     * @param input The string to hash
     * @param salt The salt to use in hashing
     * @return The hashed string
     */
    std::string customHash(const std::string& input, const std::string& salt) const;
    
    /**
     * @brief Generates a random salt of specified length
     * @param length The length of the salt to generate
     * @return The generated salt string
     */
    std::string generateSalt(size_t length = 16) const;
    
    /**
     * @brief Encrypts a password using the provided key
     * @param password The password to encrypt
     * @param key The encryption key (typically master password hash)
     * @return The encrypted password
     */
    std::string encryptPassword(const std::string& password, const std::string& key) const;
    
    /**
     * @brief Decrypts an encrypted password using the provided key
     * @param encryptedPassword The encrypted password to decrypt
     * @param key The decryption key (typically master password hash)
     * @return The decrypted password
     */
    std::string decryptPassword(const std::string& encryptedPassword, const std::string& key) const;
    
    /**
     * @brief Generates a random password of specified length
     * @param length The length of the password to generate
     * @return The generated password
     */
    std::string generatePassword(size_t length = 16) const;
};

} // namespace passman