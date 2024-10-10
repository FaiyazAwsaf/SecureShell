// CaesarCipher.h - Concrete class for Caesar Cipher encryption
#ifndef CAESAR_CIPHER_H
#define CAESAR_CIPHER_H

#include <iostream>
#include "ICryptography.h"

class CaesarCipher : public ICryptography {
private:
    int shift;
public:
    CaesarCipher(int shift) : shift(shift) {}


    std::string encrypt(const std::string &data) const override {
        std::string result = data;
        for (char &c : result) {
            if (isalpha(c)) {
                char base = islower(c) ? 'a' : 'A';
                c = (c - base + shift) % 26 + base;
            }
        }
        return result;
    }

    std::string decrypt(const std::string &data) const override {
        std::string result = data;
        for (char &c : result) {
            if (isalpha(c)) {
                char base = islower(c) ? 'a' : 'A';
                c = (c - base - shift + 26) % 26 + base;
            }
        }
        return result;
    }

    std::string hash(const std::string &data) const override {
        int hashValue = 0;
        for (char c : data) {
            hashValue += static_cast<int>(c);
        }
        return std::to_string(hashValue);
    }
};

#endif // CAESAR_CIPHER_H
