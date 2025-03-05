#include "utils/Utils.h"
#include <algorithm>
#include <random>
#include <sstream>
#include <filesystem>

namespace Utils {
    bool fileExists(const std::string& path) {
        return std::filesystem::exists(path);
    }

    std::string getFileExtension(const std::string& filename) {
        size_t pos = filename.find_last_of('.');
        return (pos != std::string::npos) ? filename.substr(pos) : "";
    }

    std::string getCurrentDirectory() {
        return std::filesystem::current_path().string();
    }

    bool changeDirectory(const std::string& path) {
        try {
            std::filesystem::current_path(path);
            return true;
        } catch (const std::filesystem::filesystem_error&) {
            return false;
        }
    }

    std::vector<std::string> listDirectory(const std::string& path) {
        std::vector<std::string> files;
        try {
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                files.push_back(entry.path().filename().string());
            }
        } catch (const std::filesystem::filesystem_error&) {}
        std::sort(files.begin(), files.end());
        return files;
    }

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

    std::string generateRandomString(size_t length) {
        static const std::string charset =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "!@#$%^&*()";

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
}