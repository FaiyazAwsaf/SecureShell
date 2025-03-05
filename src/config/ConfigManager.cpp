#include "config/ConfigManager.h"
#include <fstream>
#include <sstream>

ConfigManager::ConfigManager() {
    // Set default values
    defaultCompiler = "g++";
    autoExecute = true;
    settings["background_color"] = "Dark";
    settings["password_storage"] = "encrypted";
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
    file << "background_color" << CONFIG_DELIMITER << colorToString(getBackgroundColor()) << '\n';
    file << "password_storage" << CONFIG_DELIMITER << getPasswordStorageMethod() << '\n';

    return true;
}

void ConfigManager::setBackgroundColor(BackgroundColor color) {
    settings["background_color"] = colorToString(color);
}

ConfigManager::BackgroundColor ConfigManager::getBackgroundColor() const {
    auto it = settings.find("background_color");
    if (it != settings.end()) {
        return stringToColor(it->second);
    }
    return BackgroundColor::Dark; // Default color
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
    } else if (key == "background_color") {
        settings["background_color"] = value;
    } else if (key == "password_storage") {
        settings["password_storage"] = value;
    }

    return true;
}

std::string ConfigManager::colorToString(BackgroundColor color) const {
    switch (color) {
        case BackgroundColor::Dark: return "Dark";
        case BackgroundColor::Bright: return "Bright";
        case BackgroundColor::DarkBlue: return "DarkBlue";
        case BackgroundColor::DarkGreen: return "DarkGreen";
        case BackgroundColor::DarkRed: return "DarkRed";
        default: return "Dark";
    }
}

ConfigManager::BackgroundColor ConfigManager::stringToColor(const std::string& colorStr) const {
    if (colorStr == "Bright") return BackgroundColor::Bright;
    if (colorStr == "DarkBlue") return BackgroundColor::DarkBlue;
    if (colorStr == "DarkGreen") return BackgroundColor::DarkGreen;
    if (colorStr == "DarkRed") return BackgroundColor::DarkRed;
    return BackgroundColor::Dark;
}