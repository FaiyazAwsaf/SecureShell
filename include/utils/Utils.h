#pragma once

#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <filesystem>

namespace Utils {
    // File and path operations
    bool fileExists(const std::string& path);
    std::string getFileExtension(const std::string& filename);
    std::string getCurrentDirectory();
    bool changeDirectory(const std::string& path);
    std::vector<std::string> listDirectory(const std::string& path);

    // String operations
    std::string trim(const std::string& str);
    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string toLower(const std::string& str);
    std::string toUpper(const std::string& str);

    // Random number generation
    std::string generateRandomString(size_t length);
    
    // Password utilities
    std::string readMaskedPassword();
    bool validatePasswordStrength(const std::string& password);

};