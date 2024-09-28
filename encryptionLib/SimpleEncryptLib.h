#ifndef SIMPLEENCRYPTLIB_H
#define SIMPLEENCRYPTLIB_H

#include <string>
#include <sstream>

class SimpleEncryptLib {
public:
    static std::string encrypt(const std::string& text);

    static std::string decrypt(const std::string& encryptedText);

    static std::string generateRandomPassword(int length);
};

#endif // SIMPLEENCRYPTLIB_H
