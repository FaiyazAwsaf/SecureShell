#include "passman/PasswordManager.h"
#include "encryption/FileEncryption.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>

PasswordManager::PasswordManager() {}

std::string PasswordManager::customHash(const std::string& input, const std::string& salt) const {
    std::string combined = input + salt;
    std::vector<uint8_t> hash(32, 0); // 256-bit hash output
    
    // Initial mixing
    uint32_t h1 = 0x6a09e667;
    uint32_t h2 = 0xbb67ae85;
    
    // Process each character
    for (size_t i = 0; i < combined.length(); ++i) {
        h1 = ((h1 << 5) | (h1 >> 27)) + static_cast<uint8_t>(combined[i]);
        h2 = ((h2 >> 3) | (h2 << 29)) ^ static_cast<uint8_t>(combined[i]);
        
        // Additional mixing
        h1 = h1 ^ (h2 << 13);
        h2 = h2 ^ (h1 >> 7);
    }
    
    // Final mixing
    for (int i = 0; i < 4; ++i) {
        hash[i] = static_cast<uint8_t>((h1 >> (i * 8)) & 0xFF);
        hash[i + 4] = static_cast<uint8_t>((h2 >> (i * 8)) & 0xFF);
    }
    
    // Convert to hex string
    std::stringstream ss;
    for (uint8_t byte : hash) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return ss.str();
}

std::string PasswordManager::generateSalt(size_t length) const {
    static const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.length() - 1);
    
    std::string salt;
    salt.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        salt += chars[dis(gen)];
    }
    return salt;
}

bool PasswordManager::initialize(const std::string& masterPassword) {
    masterSalt = generateSalt();
    masterPasswordHash = customHash(masterPassword, masterSalt);
    return loadPasswords();
}

bool PasswordManager::authenticate(const std::string& masterPassword) const {
    return verifyMasterPassword(masterPassword);
}

bool PasswordManager::verifyMasterPassword(const std::string& inputPassword) const {
    return customHash(inputPassword, masterSalt) == masterPasswordHash;
}

bool PasswordManager::addEntry(const std::string& service, const std::string& username, const std::string& password) {
    std::string salt = generateSalt();
    PasswordEntry entry{
        username,
        customHash(password, salt),
        service,
        salt
    };
    passwords[service] = entry;
    return savePasswords();
}

bool PasswordManager::removeEntry(const std::string& service) {
    if (passwords.find(service) == passwords.end()) {
        return false;
    }
    passwords.erase(service);
    return savePasswords();
}

bool PasswordManager::updateEntry(const std::string& service, const std::string& username, const std::string& password) {
    if (passwords.find(service) == passwords.end()) {
        return false;
    }
    return addEntry(service, username, password);
}

std::vector<std::string> PasswordManager::listServices() const {
    std::vector<std::string> services;
    services.reserve(passwords.size());
    for (const auto& pair : passwords) {
        services.push_back(pair.first);
    }
    return services;
}

PasswordManager::PasswordEntry PasswordManager::getEntry(const std::string& service) const {
    auto it = passwords.find(service);
    if (it == passwords.end()) {
        return PasswordEntry{};
    }
    return it->second;
}

std::string PasswordManager::generatePassword(size_t length) const {
    static const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.length() - 1);
    
    std::string password;
    password.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        password += chars[dis(gen)];
    }
    return password;
}

bool PasswordManager::loadPasswords() {
    std::ifstream file("passwords.dat", std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    FileEncryption encryptor;
    std::string encryptedData;
    std::getline(file, encryptedData);
    file.close();

    if (encryptedData.empty()) {
        return true; // New file or empty file is not an error
    }

    std::string tempInputFile = "temp_encrypted.dat";
    std::string tempOutputFile = "temp_decrypted.dat";
    
    // Write encrypted data to temporary file
    std::ofstream tempInput(tempInputFile);
    tempInput << encryptedData;
    tempInput.close();
    
    // Decrypt the file
    if (!encryptor.decryptFile(tempInputFile, tempOutputFile, masterPasswordHash)) {
        std::remove(tempInputFile.c_str());
        return false;
    }
    
    // Read decrypted data
    std::ifstream tempOutput(tempOutputFile);
    std::stringstream buffer;
    buffer << tempOutput.rdbuf();
    std::string decryptedData = buffer.str();
    tempOutput.close();
    
    // Clean up temporary files
    std::remove(tempInputFile.c_str());
    std::remove(tempOutputFile.c_str());
    
    if (decryptedData.empty()) {
        return false;
    }

    std::istringstream stream(decryptedData);
    std::string line;
    passwords.clear();

    while (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        std::string service, username, passwordHash, salt;

        if (std::getline(lineStream, service, '|') &&
            std::getline(lineStream, username, '|') &&
            std::getline(lineStream, passwordHash, '|') &&
            std::getline(lineStream, salt)) {
            passwords[service] = PasswordEntry{username, passwordHash, service, salt};
        }
    }

    return true;
}

bool PasswordManager::savePasswords() const {
    std::ostringstream stream;
    for (const auto& pair : passwords) {
        const auto& entry = pair.second;
        stream << entry.service << '|'
               << entry.username << '|'
               << entry.passwordHash << '|'
               << entry.salt << '\n';
    }

    FileEncryption encryptor;
    
    // Write data to a temporary file first
    std::string tempFile = "temp_passwords.dat";
    std::ofstream tempStream(tempFile);
    if (!tempStream.is_open()) {
        return false;
    }
    tempStream << stream.str();
    tempStream.close();

    // Encrypt the temporary file to the final password file
    bool success = encryptor.encryptFile(tempFile, "passwords.dat", masterPasswordHash);
    
    // Clean up temporary file
    std::remove(tempFile.c_str());
    
    return success;
}

bool PasswordManager::changeMasterPassword(const std::string& oldPassword, const std::string& newPassword) {
    if (!verifyMasterPassword(oldPassword) || newPassword.empty()) {
        return false;
    }

    std::string newSalt = generateSalt();
    std::string newHash = customHash(newPassword, newSalt);

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