#include "utils/Utils.h"
#include <algorithm>
#include <filesystem>
#include <iostream>

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
}