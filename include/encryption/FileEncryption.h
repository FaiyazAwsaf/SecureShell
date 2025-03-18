#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <cstdint>

class EncryptionHandler;

class FileEncryption {
public:
    FileEncryption();
    ~FileEncryption();

    bool encryptFile(const std::string& inputFile, const std::string& password, const std::string& outputFile = "") const;
    bool decryptFile(const std::string& inputFile, const std::string& password, const std::string& outputFile = "") const;
    bool isFileEncrypted(const std::string& filename) const;

private:
    bool readFile(const std::string& filename, std::vector<uint8_t>& data) const;
    bool writeFile(const std::string& filename, const std::vector<uint8_t>& data) const;

    EncryptionHandler* encryptionHandler;
    const std::string ENCRYPTION_MARKER = "ENCRYPTED_";
    const size_t BLOCK_SIZE = 1024;
};