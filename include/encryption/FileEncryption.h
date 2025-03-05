#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <cstdint>

class FileEncryption {
public:
    FileEncryption();
    ~FileEncryption() = default;

    bool encryptFile(const std::string& inputFile, const std::string& outputFile, const std::string& password);
    bool decryptFile(const std::string& inputFile, const std::string& outputFile, const std::string& password);
    bool isFileEncrypted(const std::string& filename) const;

private:
    std::vector<uint8_t> xorEncrypt(const std::vector<uint8_t>& data, const std::string& key) const;
    std::vector<uint8_t> caesarEncrypt(const std::vector<uint8_t>& data, int shift) const;
    std::vector<uint8_t> caesarDecrypt(const std::vector<uint8_t>& data, int shift) const;
    int generateShift(const std::string& password) const;
    bool readFile(const std::string& filename, std::vector<uint8_t>& data) const;
    bool writeFile(const std::string& filename, const std::vector<uint8_t>& data) const;
    std::string generateFileKey(const std::string& password) const;

    const std::string ENCRYPTION_MARKER = "ENCRYPTED_";
    const size_t BLOCK_SIZE = 1024;
};