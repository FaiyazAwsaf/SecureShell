#include "terminal/Terminal.h"
#include "utils/Utils.h"
#include "passman/PasswordManager.h"
#include "terminal/CommandImplementation.h"
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <conio.h>
#include <windows.h>

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

Terminal::~Terminal() = default;

void Terminal::start() {
    running = true;

    setConsoleColor(FOREGROUND_GREEN);
    std::cout << "--------------------------------------------------------------------\n\n";
    std::cout << "                  Welcome to SecureShell Terminal!                  \n\n";
    std::cout << "--------------------------------------------------------------------\n\n";

    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    std::cout << "Type 'help' for a list of available commands.\n\n";

    while (running) {
        displayPrompt();

        std::string input;
        char ch;
        while ((ch = _getch()) != '\r') {
            if (ch == -32 || ch == 0) {
                _getch();
                continue;
            }

            if (ch == '\b') {
                if (!input.empty()) {
                    input.pop_back();
                    std::cout << "\b \b";
                }
            } else {
                input += ch;

                bool isCommand = false;
                for (const auto& cmd : commandParser->getCommandList()) {
                    if (input == cmd.first.substr(0, input.length())) {
                        isCommand = true;
                        break;
                    }
                }

                if (isCommand) {
                    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN);
                } else {
                    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }

                std::cout << ch;
            }
        }

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
    commandParser->registerCommand("run", [this](const auto& args) { commandImpl->compile(args); });
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
    commandParser->registerCommand("write", [this](const auto& args){ commandImpl->write(args); });
    commandParser->registerCommand("grep", [this](const auto& args) { commandImpl->grep(args); });
    commandParser->registerCommand("head", [this](const auto& args) { commandImpl->head(args); });
    commandParser->registerCommand("tree", [this](const auto& args) { commandImpl->tree(args); });
    commandParser->registerCommand("find", [this](const auto& args) { commandImpl->find(args); });
    commandParser->registerCommand("sysinfo", [this](const auto& args) { commandImpl->system_info(args); });
    commandParser->registerCommand("stat", [this](const auto& args){ commandImpl->stat(args);});
}

void Terminal::displayPrompt() const {
    setConsoleColor(FOREGROUND_GREEN | FOREGROUND_RED);
    std::cout << std::filesystem::current_path().string();
    std::cout << "> ";
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

}

void Terminal::executeCommand(const std::string& command, const std::vector<std::string>& args) {
    if (!commandParser->executeCommand(command, args)) {
        std::cout << "Unknown command: " << command << "\n";
        std::cout << "Type 'help' for a list of available commands.\n";
    }
}
