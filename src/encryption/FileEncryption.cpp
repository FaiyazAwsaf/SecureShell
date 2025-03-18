#include "encryption/FileEncryption.h"
#include "encryption/EncryptionHandler.h"
#include <fstream>
#include <stdexcept>
#include <vector>

FileEncryption::FileEncryption() : encryptionHandler(new EncryptionHandler()) {}

FileEncryption::~FileEncryption() {
    delete encryptionHandler;
}

bool FileEncryption::readFile(const std::string& filename, std::vector<uint8_t>& data) const {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        return false;
    }
    
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    data.resize(fileSize);
    file.read(reinterpret_cast<char*>(data.data()), fileSize);
    
    return true;
}

bool FileEncryption::writeFile(const std::string& filename, const std::vector<uint8_t>& data) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    return true;
}

bool FileEncryption::isFileEncrypted(const std::string& filename) const {
    std::vector<uint8_t> data;
    if (!readFile(filename, data) || data.size() < ENCRYPTION_MARKER.length()) {
        return false;
    }
    
    try {
        for (int testShift = 1; testShift <= 255; testShift++) {
            auto caesarResult = encryptionHandler->caesarDecrypt(data, testShift);
            
            if (caesarResult.size() >= ENCRYPTION_MARKER.length()) {
                return true;
            }
        }
    } catch (const std::exception&) {
    }
    
    return false;
}

bool FileEncryption::encryptFile(const std::string& inputFile, const std::string& outputFile, const std::string& password) const {
    try {
        std::vector<uint8_t> data;
        if (!readFile(inputFile, data)) {
            return false;
        }
        
        std::vector<uint8_t> markerData(ENCRYPTION_MARKER.begin(), ENCRYPTION_MARKER.end());
        std::vector<uint8_t> finalData = markerData;
        finalData.insert(finalData.end(), data.begin(), data.end());
        
        // Use EncryptionHandler for encryption operations
        std::string key = encryptionHandler->generateFileKey(password, BLOCK_SIZE);
        int shift = encryptionHandler->generateShift(password);
        
        auto xorResult = encryptionHandler->xorEncrypt(finalData, key);
        auto caesarResult = encryptionHandler->caesarEncrypt(xorResult, shift);
        
        return writeFile(outputFile, caesarResult);
    } catch (const std::exception& e) {
        return false;
    }
}

bool FileEncryption::decryptFile(const std::string& inputFile, const std::string& outputFile, const std::string& password) const {
    try {
        std::vector<uint8_t> data;
        if (!readFile(inputFile, data)) {
            return false;
        }
        
        if (data.size() < ENCRYPTION_MARKER.length()) {
            return false;
        }
        
        // Use EncryptionHandler for decryption operations
        std::string key = encryptionHandler->generateFileKey(password, BLOCK_SIZE);
        int shift = encryptionHandler->generateShift(password);
        
        auto caesarResult = encryptionHandler->caesarDecrypt(data, shift);
        auto xorResult = encryptionHandler->xorEncrypt(caesarResult, key); // XOR is its own inverse
        
        if (xorResult.size() < ENCRYPTION_MARKER.length()) {
            return false;
        }
        
        std::string decryptedMarker(xorResult.begin(), xorResult.begin() + ENCRYPTION_MARKER.length());
        if (decryptedMarker != ENCRYPTION_MARKER) {
            return false;
        }
        
        std::vector<uint8_t> finalData(xorResult.begin() + ENCRYPTION_MARKER.length(), xorResult.end());
        
        return writeFile(outputFile, finalData);
    } catch (const std::exception& e) {
        return false;
    }
}