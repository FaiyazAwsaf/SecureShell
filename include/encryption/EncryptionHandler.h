#pragma once

#include <string>
#include <vector>
#include <cstdint>

class EncryptionHandler {
public:
    EncryptionHandler();
    ~EncryptionHandler() = default;

    std::vector<uint8_t> xorEncrypt(const std::vector<uint8_t>& data, const std::string& key) const;
    std::vector<uint8_t> caesarEncrypt(const std::vector<uint8_t>& data, int shift) const;
    std::vector<uint8_t> caesarDecrypt(const std::vector<uint8_t>& data, int shift) const;
    int generateShift(const std::string& password) const;
    std::string generateFileKey(const std::string& password, size_t blockSize) const;
};