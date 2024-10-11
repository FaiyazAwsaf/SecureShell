// XORCipher.h - Concrete class for XOR encryption
#ifndef XOR_CIPHER_H
#define XOR_CIPHER_H

#include "ICryptography.h"

class XORCipher : public ICryptography {
private:
    char key;
public:
    XORCipher(char key) : key(key) {}

    std::string encrypt(const std::string &data) const override {
        std::string result = data;
        for (char &c : result) {
            c ^= key; // XOR each character with the key
        }
        return result;
    }

    std::string decrypt(const std::string &data) const override {
        // XOR is reversible with the same key
        return encrypt(data);
    }

    std::string hash(const std::string &data) const override {
        int hashValue = 0;
        for (char c : data) {
            hashValue += static_cast<int>(c);
        }
        return std::to_string(hashValue);
    }
};

#endif // XOR_CIPHER_H
