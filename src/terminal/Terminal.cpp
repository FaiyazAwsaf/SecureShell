#include "terminal/Terminal.h"
#include "utils/Utils.h"
#include <iostream>
#include <filesystem>
#include <algorithm>

Terminal::Terminal()
    : running(false),
      commandParser(std::make_unique<CommandParser>()),
      configManager(std::make_unique<ConfigManager>()) {
    initializeCommands();
    configManager->loadConfig();
}

void Terminal::start() {
    running = true;
    std::cout << "Welcome to SecureShell Terminal!\n";
    std::cout << "Type 'help' for a list of available commands.\n";

    while (running) {
        displayPrompt();
        std::string input;
        std::getline(std::cin, input);

        if (!input.empty()) {
            processCommand(input);
        }
    }
}

void Terminal::stop() {
    running = false;
}

bool Terminal::isRunning() const {
    return running;
}

void Terminal::processCommand(const std::string& input) {
    auto tokens = commandParser->parseInput(input);
    if (tokens.empty()) return;

    std::string command = tokens[0];
    std::vector<std::string> args(tokens.begin() + 1, tokens.end());

    // Check for aliases
    auto aliasIt = aliases.find(command);
    if (aliasIt != aliases.end()) {
        command = aliasIt->second;
    }

    executeCommand(command, args);
}

void Terminal::initializeCommands() {
    commandParser->registerCommand("help", [this](const auto& args) { displayHelp(); });
    commandParser->registerCommand("exit", [this](const auto& args) { stop(); });
    commandParser->registerCommand("cd", [this](const auto& args) {
        if (args.empty()) {
            std::cout << "Usage: cd <directory>\n";
            return;
        }
        if (!Utils::changeDirectory(args[0])) {
            std::cout << "Failed to change directory\n";
        }
    });
    commandParser->registerCommand("ls", [](const auto& args) {
        std::string path = args.empty() ? "." : args[0];
        auto files = Utils::listDirectory(path);
        for (const auto& file : files) {
            std::cout << file << "\n";
        }
    });
    commandParser->registerCommand("compile", [this](const auto& args) {
        if (args.empty()) {
            std::cout << "Usage: compile <filename>\n";
            return;
        }
        compileAndRun(args[0]);
    });
    // Add more commands here...
}

void Terminal::displayPrompt() const {
    std::cout << std::filesystem::current_path().string() << "> ";
}

void Terminal::executeCommand(const std::string& command, const std::vector<std::string>& args) {
    if (!commandParser->executeCommand(command, args)) {
        std::cout << "Unknown command: " << command << "\n";
        std::cout << "Type 'help' for a list of available commands.\n";
    }
}

void Terminal::compileAndRun(const std::string& filename) {
    std::string ext = Utils::getFileExtension(filename);
    std::string compiler = configManager->getDefaultCompiler();
    bool autoExecute = configManager->getAutoExecute();

    // Add compilation logic here based on file extension
    // This is a simplified example
    if (ext == ".cpp" || ext == ".cc") {
        std::string outfile = filename.substr(0, filename.length() - ext.length());
        #ifdef _WIN32
        outfile += ".exe";
        #endif

        std::string command = compiler + " " + filename + " -o " + outfile;
        int result = system(command.c_str());

        if (result == 0 && autoExecute) {
            system(outfile.c_str());
        }
    }
    // Add support for other file types...
}

void Terminal::displayHelp() const {
    std::cout << "Available commands:\n";
    auto commands = commandParser->getCommandList();
    for (const auto& [cmd, desc] : commands) {
        std::cout << cmd << "\t" << desc << "\n";
    }
}