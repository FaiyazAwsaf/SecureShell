#include "SimpleEncryptLib.h"
#include <cstdlib>
#include <ctime>

std::string SimpleEncryptLib::encrypt(const std::string& input) {
    std::stringstream encryptedStream;
    for (char c : input) {
        encryptedStream << int(c) << " "; // Convert to ascii and add  space
    }
    return encryptedStream.str();
}

std::string SimpleEncryptLib::decrypt(const std::string& input) {
    std::stringstream inputStream(input);
    std::string decrypted;
    int asciiValue;

    while (inputStream >> asciiValue) {
        decrypted += char(asciiValue); 
    }

    return decrypted;
}

std::string SimpleEncryptLib::generateRandomPassword(int length) {
    const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string password;
    
    std::srand(std::time(0));

    for (int i = 0; i < length; i++) {
        int randomIndex = rand() % charset.size(); 
        password += charset[randomIndex];
    }
    return password;
}
