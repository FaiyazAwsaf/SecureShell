#include "config/ConfigManager.h"
#include <fstream>
#include <sstream>

ConfigManager::ConfigManager() {
    defaultCompiler = "g++";
    autoExecute = true;
}

bool ConfigManager::loadConfig(const std::string& configFile) {
    configFilePath = configFile;
    std::ifstream file(configFile);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!parseConfigLine(line)) {
            return false;
        }
    }
    return true;
}

bool ConfigManager::saveConfig() const {
    std::ofstream file(configFilePath);
    if (!file.is_open()) {
        return false;
    }

    file << "compiler" << CONFIG_DELIMITER << defaultCompiler << '\n';
    file << "auto_execute" << CONFIG_DELIMITER << (autoExecute ? "true" : "false") << '\n';
    file << "password_storage" << CONFIG_DELIMITER << getPasswordStorageMethod() << '\n';

    return true;
}



void ConfigManager::setDefaultCompiler(const std::string& compiler) {
    defaultCompiler = compiler;
}

std::string ConfigManager::getDefaultCompiler() const {
    return defaultCompiler;
}

void ConfigManager::setAutoExecute(bool enabled) {
    autoExecute = enabled;
}

bool ConfigManager::getAutoExecute() const {
    return autoExecute;
}

void ConfigManager::setPasswordStorageMethod(const std::string& method) {
    settings["password_storage"] = method;
}

std::string ConfigManager::getPasswordStorageMethod() const {
    auto it = settings.find("password_storage");
    if (it != settings.end()) {
        return it->second;
    }
    return "encrypted"; // Default method
}

bool ConfigManager::parseConfigLine(const std::string& line) {
    if (line.empty() || line[0] == '#') { // Skip empty lines and comments
        return true;
    }

    size_t delimiterPos = line.find(CONFIG_DELIMITER);
    if (delimiterPos == std::string::npos) {
        return false;
    }

    std::string key = line.substr(0, delimiterPos);
    std::string value = line.substr(delimiterPos + 1);

    if (key == "compiler") {
        defaultCompiler = value;
    } else if (key == "auto_execute") {
        autoExecute = (value == "true");
    } else if (key == "password_storage") {
        settings["password_storage"] = value;
    }

    return true;
}