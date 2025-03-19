#include "passman/PasswordStorage.h"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace passman {

PasswordStorage::PasswordStorage(const std::string& dataDir)
    : dataDir(dataDir),
      passwordFile(dataDir + "passwords.txt"),
      masterFile(dataDir + "master.txt") {
    std::filesystem::create_directories(dataDir);
}

bool PasswordStorage::loadMasterPassword(std::string& masterPasswordHash, std::string& masterSalt) const {
    std::ifstream masterFile(this->masterFile);
    if (!masterFile.is_open()) {
        return false; // File doesn't exist or can't be opened
    }
    
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
    std::filesystem::create_directories(dataDir);
    
    std::ofstream masterFileStream(masterFile);
    if (!masterFileStream.is_open()) {
        return false;
    }
    
    masterFileStream << masterPasswordHash << '\n' << masterSalt;
    masterFileStream.close();
    return true;
}

    // Optimized to decrypt password data in memory instead of temp files
bool PasswordStorage::loadPasswords(
    std::unordered_map<std::string, PasswordEntry>& passwords,
    const std::string& masterPasswordHash) const {
    
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
    
    std::ofstream tempInput(tempInputFile);
    tempInput << encryptedData;
    tempInput.close();
    
    if (!encryptor.decryptFile(tempInputFile, tempOutputFile, masterPasswordHash)) {
        std::remove(tempInputFile.c_str());
        return false;
    }
    
    std::ifstream tempOutput(tempOutputFile);
    std::stringstream buffer;
    buffer << tempOutput.rdbuf();
    std::string decryptedData = buffer.str();
    tempOutput.close();
    
    std::remove(tempInputFile.c_str());
    std::remove(tempOutputFile.c_str());
    
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
    
    std::filesystem::create_directories(dataDir);
    
    std::ostringstream stream;
    for (const auto& pair : passwords) {
        const auto& entry = pair.second;
        stream << entry.service << '|'
               << entry.username << '|'
               << entry.encryptedPassword << '|'
               << entry.serviceLink << '|'
               << entry.salt << '\n';
    }

    std::string tempFile = dataDir + "temp_passwords.dat";
    std::ofstream tempStream(tempFile);
    if (!tempStream.is_open()) {
        return false;
    }
    tempStream << stream.str();
    tempStream.close();

    bool success = encryptor.encryptFile(tempFile, masterPasswordHash, passwordFile);
    
    std::remove(tempFile.c_str());
    
    return success;
}

} // namespace passman