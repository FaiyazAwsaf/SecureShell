#include "encryption/FileEncryption.h"
#include <fstream>
#include <stdexcept>
#include <vector>

FileEncryption::FileEncryption() {}

std::vector<uint8_t> FileEncryption::xorEncrypt(const std::vector<uint8_t>& data, const std::string& key) const {
    std::vector<uint8_t> result = data;
    const size_t keyLength = key.length();
    
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ static_cast<uint8_t>(key[i % keyLength]);
    }
    
    return result;
}

std::vector<uint8_t> FileEncryption::caesarEncrypt(const std::vector<uint8_t>& data, int shift) const {
    std::vector<uint8_t> result = data;
    
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = static_cast<uint8_t>((static_cast<int>(data[i]) + shift) % 256);
    }
    
    return result;
}

std::vector<uint8_t> FileEncryption::caesarDecrypt(const std::vector<uint8_t>& data, int shift) const {
    std::vector<uint8_t> result = data;
    
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = static_cast<uint8_t>((static_cast<int>(data[i]) - shift + 256) % 256);
    }
    
    return result;
}

int FileEncryption::generateShift(const std::string& password) const {
    int shift = 0;
    for (char c : password) {
        shift += static_cast<int>(c);
    }
    return (shift % 255) + 1; // Ensure shift is between 1-255
}

std::string FileEncryption::generateFileKey(const std::string& password) const {
    // Simple key derivation from password
    std::string key = password;
    while (key.length() < BLOCK_SIZE) {
        key += password;
    }
    return key.substr(0, BLOCK_SIZE);
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
    
    std::string marker(data.begin(), data.begin() + ENCRYPTION_MARKER.length());
    return marker == ENCRYPTION_MARKER;
}

bool FileEncryption::encryptFile(const std::string& inputFile, const std::string& outputFile, const std::string& password) {
    try {
        std::vector<uint8_t> data;
        if (!readFile(inputFile, data)) {
            return false;
        }
        
        // Add encryption marker
        std::vector<uint8_t> markerData(ENCRYPTION_MARKER.begin(), ENCRYPTION_MARKER.end());
        std::vector<uint8_t> finalData = markerData;
        finalData.insert(finalData.end(), data.begin(), data.end());
        
        // Generate encryption parameters
        std::string key = generateFileKey(password);
        int shift = generateShift(password);
        
        // Apply encryption
        auto xorResult = xorEncrypt(finalData, key);
        auto caesarResult = caesarEncrypt(xorResult, shift);
        
        return writeFile(outputFile, caesarResult);
    } catch (const std::exception& e) {
        return false;
    }
}

bool FileEncryption::decryptFile(const std::string& inputFile, const std::string& outputFile, const std::string& password) {
    try {
        std::vector<uint8_t> data;
        if (!readFile(inputFile, data)) {
            return false;
        }
        
        // Verify encryption marker
        if (data.size() < ENCRYPTION_MARKER.length()) {
            return false;
        }
        
        // Generate decryption parameters
        std::string key = generateFileKey(password);
        int shift = generateShift(password);
        
        // Apply decryption
        auto caesarResult = caesarDecrypt(data, shift);
        auto xorResult = xorEncrypt(caesarResult, key); // XOR is its own inverse
        
        // Remove encryption marker
        std::vector<uint8_t> finalData(xorResult.begin() + ENCRYPTION_MARKER.length(), xorResult.end());
        
        return writeFile(outputFile, finalData);
    } catch (const std::exception& e) {
        return false;
    }
}