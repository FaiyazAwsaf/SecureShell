#include "encryption/EncryptionHandler.h"
#include <stdexcept>

EncryptionHandler::EncryptionHandler() {}

std::vector<uint8_t> EncryptionHandler::xorEncrypt(const std::vector<uint8_t>& data, const std::string& key) const {
    std::vector<uint8_t> result = data;
    const size_t keyLength = key.length();
    
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ static_cast<uint8_t>(key[i % keyLength]);
    }
    
    return result;
}

std::vector<uint8_t> EncryptionHandler::caesarEncrypt(const std::vector<uint8_t>& data, int shift) const {
    std::vector<uint8_t> result = data;
    
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = static_cast<uint8_t>((static_cast<int>(data[i]) + shift) % 256);
    }
    
    return result;
}

std::vector<uint8_t> EncryptionHandler::caesarDecrypt(const std::vector<uint8_t>& data, int shift) const {
    std::vector<uint8_t> result = data;
    
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = static_cast<uint8_t>((static_cast<int>(data[i]) - shift + 256) % 256);
    }
    
    return result;
}

int EncryptionHandler::generateShift(const std::string& password) const {
    int shift = 0;
    for (char c : password) {
        shift += static_cast<int>(c);
    }
    return (shift % 255) + 1; // Ensure shift is between 1-255
}

std::string EncryptionHandler::generateFileKey(const std::string& password, size_t blockSize) const {
    std::string key = password;
    while (key.length() < blockSize) {
        key += password;
    }
    return key.substr(0, blockSize);
}