#include "utils/Utils.h"
#include <algorithm>
#include <sstream>
#include <regex>
#include <iostream>
#include <conio.h> 
namespace Utils {
    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, last - first + 1);
    }

    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delimiter)) {
            if (!token.empty()) {
                tokens.push_back(trim(token));
            }
        }
        return tokens;
    }

    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::toupper(c); });
        return result;
    }

    // Improved random string generation
    std::string generateRandomString(size_t length) {
        static const std::string charset =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "!@#$%^&*()-=_+";

        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<size_t> distribution(0, charset.size() - 1);

        std::string result;
        result.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            result += charset[distribution(generator)];
        }
        return result;
    }
    
    std::string readMaskedPassword() {
        std::string password;
        char ch;
        while ((ch = _getch()) != '\r') {
            if (ch == '\b') {
                if (!password.empty()) {
                    password.pop_back();
                    std::cout << "\b \b";
                }
            } else if (ch >= 32 && ch <= 126) {
                password.push_back(ch);
                std::cout << '*';
            }
        }
        std::cout << std::endl;
        return password;
    }
    
    // Now enforces uppercase and lowercase letters
    bool validatePasswordStrength(const std::string& password) {
        if (password.length() < 8) {
            return false;
        }
        
        bool hasLetter = std::regex_search(password, std::regex("[a-zA-Z]"));
        
        bool hasDigit = std::regex_search(password, std::regex("[0-9]"));
        
        bool hasSpecial = std::regex_search(password, std::regex("[!@#$%^&*-=_+]"));

        return hasLetter && hasDigit && hasSpecial;
    }
}