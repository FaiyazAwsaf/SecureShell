#pragma once

#include <string>
#include <unordered_map>

class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager() = default;

    bool loadConfig(const std::string& configFile = "terminal_config.txt");
    bool saveConfig() const;

    void setDefaultCompiler(const std::string& compiler);
    std::string getDefaultCompiler() const;
    void setAutoExecute(bool enabled);
    bool getAutoExecute() const;
    void setPasswordStorageMethod(const std::string& method);
    std::string getPasswordStorageMethod() const;

private:
    bool parseConfigLine(const std::string& line);

    std::string configFilePath;
    std::string defaultCompiler;
    bool autoExecute;
    std::unordered_map<std::string, std::string> settings;
    const std::string CONFIG_DELIMITER = "=";
};