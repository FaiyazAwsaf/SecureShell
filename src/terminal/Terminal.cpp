#include "terminal/Terminal.h"
#include "utils/Utils.h"
#include "passman/PasswordManager.h"
#include "terminal/CommandImplementation.h"  // Add this include
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <conio.h> // For _getch()
#include <windows.h> // For Windows console color

// Function to set console text color
void setConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}


Terminal::Terminal()
    : running(false),
      commandParser(std::make_unique<CommandParser>()),
      commandImpl(std::make_unique<CommandImplementation>(*this)) {
    initializeCommands();
}

// Add this after the constructor
Terminal::~Terminal() = default;

void Terminal::start() {
    running = true;

    // Set welcome message color to green
    setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "-------------Welcome to SecureShell Terminal!-------------\n\n";
    std::cout << "Type 'help' for a list of available commands.\n\n";

    // Reset the text color to default (gray)
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    while (running) {
        displayPrompt();

        // Read input character by character
        std::string input;
        char ch;
        while ((ch = _getch()) != '\r') { // Read until Enter is pressed
            if (ch == '\b') { // Handle backspace
                if (!input.empty()) {
                    input.pop_back();
                    std::cout << "\b \b"; // Move cursor back, overwrite with space, move back again
                }
            } else {
                input += ch;

                // Check if the input matches a command
                bool isCommand = false;
                for (const auto& cmd : commandParser->getCommandList()) {
                    if (input == cmd.first.substr(0, input.length())) {
                        isCommand = true;
                        break;
                    }
                }

                // Set color based on whether the input is a command
                if (isCommand) {
                    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Blue for commands
                } else {
                    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Default color
                }

                std::cout << ch; // Print the character
            }
        }

        // Reset color to default after input is complete
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        std::cout << std::endl;

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
    commandParser->registerCommand("help", [this](const auto& args) { commandImpl->help(); });
    commandParser->registerCommand("exit", [this](const auto& args) { commandImpl->exit(); });
    commandParser->registerCommand("cd", [this](const auto& args) { commandImpl->cd(args); });
    commandParser->registerCommand("ls", [this](const auto& args) { commandImpl->ls(args); });
    commandParser->registerCommand("compile", [this](const auto& args) { commandImpl->compile(args); });
    commandParser->registerCommand("passman", [this](const auto& args) { commandImpl->passman(args); });
    commandParser->registerCommand("copy", [this](const auto& args) { commandImpl->copy(args); });
    commandParser->registerCommand("move", [this](const auto& args) { commandImpl->move(args); });
    commandParser->registerCommand("rename", [this](const auto& args) { commandImpl->rename(args); });
    commandParser->registerCommand("dcreate", [this](const auto& args) { commandImpl->create_directory(args); });
    commandParser->registerCommand("fcreate", [this](const auto& args) { commandImpl->create_file(args); });
    commandParser->registerCommand("remove", [this](const auto& args) { commandImpl->remove(args); });
    commandParser->registerCommand("perm", [this](const auto& args) { commandImpl->display_permission(args); });
    commandParser->registerCommand("curr", [this](const auto& args) { commandImpl->get_current_directory(args); });
    commandParser->registerCommand("encrypt", [this](const auto& args) { commandImpl->encrypt(args); });
    commandParser->registerCommand("decrypt", [this](const auto& args) { commandImpl->decrypt(args); });
	commandParser->registerCommand("cat", [this](const auto& args) { commandImpl->cat(args); });
    commandParser->registerCommand("grep", [this](const auto& args) { commandImpl->grep(args); });
    commandParser->registerCommand("head", [this](const auto& args) { commandImpl->head(args); });
    commandParser->registerCommand("tree", [this](const auto& args) { commandImpl->tree(args); });
    commandParser->registerCommand("find", [this](const auto& args) { commandImpl->find(args); });
    commandParser->registerCommand("sysinfo", [this](const auto& args) { commandImpl->system_info(args); });
    commandParser->registerCommand("stat", [this](const auto& args){ commandImpl->stat(args);});
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
