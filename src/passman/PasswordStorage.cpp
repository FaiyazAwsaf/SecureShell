#include "passman/PasswordStorage.h"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace passman {

PasswordStorage::PasswordStorage(const std::string& dataDir)
    : dataDir(dataDir),
      passwordFile(dataDir + "passwords.txt"),
      masterFile(dataDir + "master.txt") {
    // Create data directory if it doesn't exist
    std::filesystem::create_directories(dataDir);
}

bool PasswordStorage::loadMasterPassword(std::string& masterPasswordHash, std::string& masterSalt) const {
    std::ifstream masterFile(this->masterFile);
    if (!masterFile.is_open()) {
        return false; // File doesn't exist or can't be opened
    }
    
    // Read master password hash and salt
    std::string storedHash, storedSalt;
    std::getline(masterFile, storedHash);
    std::getline(masterFile, storedSalt);
    masterFile.close();
    
    if (storedHash.empty() || storedSalt.empty()) {
        return false;
    }
    
    masterPasswordHash = storedHash;
    masterSalt = storedSalt;
    return true;
}

bool PasswordStorage::saveMasterPassword(const std::string& masterPasswordHash, const std::string& masterSalt) const {
    // Create data directory if it doesn't exist
    std::filesystem::create_directories(dataDir);
    
    std::ofstream masterFileStream(masterFile);
    if (!masterFileStream.is_open()) {
        return false;
    }
    
    masterFileStream << masterPasswordHash << '\n' << masterSalt;
    masterFileStream.close();
    return true;
}

bool PasswordStorage::loadPasswords(
    std::unordered_map<std::string, PasswordEntry>& passwords,
    const std::string& masterPasswordHash) const {
    
    // Check if password file exists
    std::ifstream file(passwordFile, std::ios::binary);
    if (!file.is_open()) {
        return true; // New file is not an error
    }

    std::string encryptedData;
    std::getline(file, encryptedData);
    file.close();

    if (encryptedData.empty()) {
        return true; // Empty file is not an error
    }

    std::string tempInputFile = dataDir + "temp_encrypted.dat";
    std::string tempOutputFile = dataDir + "temp_decrypted.dat";
    
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
    
    // Parse decrypted data
    std::istringstream dataStream(decryptedData);
    std::string line;
    while (std::getline(dataStream, line)) {
        std::istringstream lineStream(line);
        std::string service, username, encryptedPassword, serviceLink, salt;
        
        if (std::getline(lineStream, service, '|') &&
            std::getline(lineStream, username, '|') &&
            std::getline(lineStream, encryptedPassword, '|') &&
            std::getline(lineStream, serviceLink, '|') &&
            std::getline(lineStream, salt)) {
            passwords[service] = PasswordEntry{
                service, username, encryptedPassword, serviceLink, salt
            };
        }
    }

    return true;
}

bool PasswordStorage::savePasswords(
    const std::unordered_map<std::string, PasswordEntry>& passwords,
    const std::string& masterPasswordHash) const {
    
    // Create data directory if it doesn't exist
    std::filesystem::create_directories(dataDir);
    
    // Prepare data to be saved
    std::ostringstream stream;
    for (const auto& pair : passwords) {
        const auto& entry = pair.second;
        stream << entry.service << '|'
               << entry.username << '|'
               << entry.encryptedPassword << '|'
               << entry.serviceLink << '|'
               << entry.salt << '\n';
    }

    // Write data to a temporary file first
    std::string tempFile = dataDir + "temp_passwords.dat";
    std::ofstream tempStream(tempFile);
    if (!tempStream.is_open()) {
        return false;
    }
    tempStream << stream.str();
    tempStream.close();

    // Encrypt the temporary file to the final password file
    bool success = encryptor.encryptFile(tempFile, passwordFile, masterPasswordHash);
    
    // Clean up temporary file
    std::remove(tempFile.c_str());
    
    return success;
}

} // namespace passman