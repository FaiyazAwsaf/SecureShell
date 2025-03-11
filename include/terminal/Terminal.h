#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include "CommandParser.h"

class Terminal {
public:
    Terminal();
    ~Terminal() = default;

    void start();
    void stop();
    bool isRunning() const;
    


private:
    void processCommand(const std::string& input);
    void initializeCommands();
    void displayPrompt() const;
    void executeCommand(const std::string& command, const std::vector<std::string>& args);
    void compileAndRun(const std::string& filename);
    void displayHelp() const;


    bool running;
    std::unique_ptr<CommandParser> commandParser;
    std::unordered_map<std::string, std::string> aliases;
};