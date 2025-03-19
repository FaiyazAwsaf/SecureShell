#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

class CommandParser {
    friend class
    CommandParserTest;
public:
    using CommandFunction = std::function<void(const std::vector<std::string>&)>;

    CommandParser();
    ~CommandParser() = default;

    void registerCommand(const std::string& command, CommandFunction handler);
    bool executeCommand(const std::string& command, const std::vector<std::string>& args);
    std::vector<std::string> parseInput(const std::string& input) const;
    bool isValidCommand(const std::string& command) const;
    std::vector<std::pair<std::string, std::string>> getCommandList() const;

private:
    std::unordered_map<std::string, CommandFunction> commands;
    std::unordered_map<std::string, std::string> commandDescriptions;

    void initializeDefaultCommands();
    std::vector<std::string> splitString(const std::string& input, char delimiter = ' ') const;
};