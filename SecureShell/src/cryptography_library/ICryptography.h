// ICryptography.h - Interface for cryptography algorithms
#ifndef ICRYPTOGRAPHY_H
#define ICRYPTOGRAPHY_H

#include <string>

class ICryptography {
public:
    virtual ~ICryptography() = default;
    virtual std::string encrypt(const std::string &data) const = 0;
    virtual std::string decrypt(const std::string &data) const = 0;
    virtual std::string hash(const std::string &data) const = 0;
};

#endif // ICRYPTOGRAPHY_H
