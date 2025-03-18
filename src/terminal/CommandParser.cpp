#include "terminal/CommandParser.h"
#include "utils/Utils.h"
#include <algorithm>
#include <iostream>
#include <sstream>

CommandParser::CommandParser() {
    initializeDefaultCommands();
}

    // Removed unnecessary empty descriptions for commands
void CommandParser::registerCommand(const std::string& command, CommandFunction handler) {
    commands[command] = handler;
    
    if (commandDescriptions.find(command) == commandDescriptions.end()) {
        commandDescriptions[command] = "";
    }
}

bool CommandParser::executeCommand(const std::string& command, const std::vector<std::string>& args) {
    auto it = commands.find(command);
    if (it == commands.end()) {
        return false;
    }

    try {
        it->second(args);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error executing command: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> CommandParser::parseInput(const std::string& input) const {
    std::vector<std::string> tokens;
    std::string token;
    bool inQuotes = false;
    std::stringstream tokenStream;

    for (char c : input) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ' ' && !inQuotes) {
            token = tokenStream.str();
            if (!token.empty()) {
                tokens.push_back(token);
                tokenStream.str("");
                tokenStream.clear();
            }
        } else {
            tokenStream << c;
        }
    }

    token = tokenStream.str();
    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}

bool CommandParser::isValidCommand(const std::string& command) const {
    return commands.find(command) != commands.end();
}

std::vector<std::pair<std::string, std::string>> CommandParser::getCommandList() const {
    std::vector<std::pair<std::string, std::string>> commandList;
    for (const auto& [cmd, desc] : commandDescriptions) {
        commandList.emplace_back(cmd, desc);
    }
    std::sort(commandList.begin(), commandList.end());
    return commandList;
}

void CommandParser::initializeDefaultCommands() {
    commandDescriptions = {
        {"help", "Display this help message"},
        {"exit", "Exit the terminal"},
        {"cd", "Change current directory"},
        {"ls", "List directory contents"},
        {"run", "Compile and optionally run a source file"},
        {"encrypt", "Encrypt a file with a password"},
        {"decrypt", "Decrypt a file with a password"},
        {"passman", "Access the password manager"},
        {"alias", "Create or list command aliases"},
        {"copy", "Copy a file to another location"},
        {"move", "Move a file to another location"},
        {"rename", "Rename a file or directory"},
        {"dcreate", "Create a new directory"},
        {"fcreate", "Create a new file"},
        {"remove", "Remove a file or directory"},
        {"perm", "Display file permissions"},
        {"curr", "Show current working directory"},
        {"cat", "Display contents of a file"},
        {"grep", "Search for a pattern in a file"},
        {"head", "Display first lines of a file"},
        {"tree", "Display directory structure as a tree"},
        {"find", "Find files matching a pattern"},
        {"sysinfo", "Display system information"},
        {"stat", "Display file or directory status"},
    };
    
    for (const auto& [cmd, _] : commandDescriptions) {
        commands[cmd] = [](const std::vector<std::string>&) {};
    }
}

std::vector<std::string> CommandParser::splitString(const std::string& input, char delimiter) const {
    return Utils::split(input, delimiter);
}