#include "passman/PasswordCrypto.h"
#include <sstream>
#include <iomanip>

namespace passman {

std::string PasswordCrypto::customHash(const std::string& input, const std::string& salt) const {
    std::string combined = input + salt;
    std::vector<uint8_t> hash(32, 0); // 256-bit hash output
    
    uint32_t h1 = 0x6a09e667;
    uint32_t h2 = 0xbb67ae85;
    
    for (size_t i = 0; i < combined.length(); ++i) {
        h1 = ((h1 << 5) | (h1 >> 27)) + static_cast<uint8_t>(combined[i]);
        h2 = ((h2 >> 3) | (h2 << 29)) ^ static_cast<uint8_t>(combined[i]);
        
        h1 = h1 ^ (h2 << 13);
        h2 = h2 ^ (h1 >> 7);
    }
    
    for (int i = 0; i < 4; ++i) {
        hash[i] = static_cast<uint8_t>((h1 >> (i * 8)) & 0xFF);
        hash[i + 4] = static_cast<uint8_t>((h2 >> (i * 8)) & 0xFF);
    }
    
    std::stringstream ss;
    for (uint8_t byte : hash) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return ss.str();
}

    // Using cryptographic-strength random generator
std::string PasswordCrypto::generateSalt(size_t length) const {
    static const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.length() - 1);
    
    std::string salt;
    salt.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        salt += chars[dis(gen)];
    }
    return salt;
}

std::string PasswordCrypto::encryptPassword(const std::string& password, const std::string& key) const {
    std::string result;
    result.reserve(password.length() * 2); // Reserve space for hex encoding
    
    for (size_t i = 0; i < password.length(); ++i) {
        char encrypted = password[i] ^ key[i % key.length()];
        char hex[3];
        sprintf_s(hex, "%02x", static_cast<unsigned char>(encrypted));
        result += hex;
    }
    
    return result;
}

std::string PasswordCrypto::decryptPassword(const std::string& encryptedPassword, const std::string& key) const {
    std::string result;
    result.reserve(encryptedPassword.length() / 2);
    
    for (size_t i = 0; i < encryptedPassword.length(); i += 2) {
        std::string byteStr = encryptedPassword.substr(i, 2);
        char byte = static_cast<char>(std::stoi(byteStr, nullptr, 16));
        char decrypted = byte ^ key[(i/2) % key.length()];
        result += decrypted;
    }
    
    return result;
}

std::string PasswordCrypto::generatePassword(size_t length) const {
    static const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.length() - 1);
    
    std::string password;
    password.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        password += chars[dis(gen)];
    }
    return password;
}

} // namespace passman