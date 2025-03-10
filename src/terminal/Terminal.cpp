#include "terminal/Terminal.h"
#include "utils/Utils.h"
#include "passman/PasswordManager.h"
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
    commandParser->registerCommand("passman", [this](const auto& args) {
        static passman::PasswordManager passwordManager;
        static bool initialized = false;
        
        // Check if master password file exists
        bool masterPasswordExists = passwordManager.hasMasterPassword();
        
        if (!masterPasswordExists && !initialized) {
            std::cout << "Password Manager - First Time Setup\n";
            std::cout << "Please create a master password: ";
            std::string masterPassword;
            std::getline(std::cin, masterPassword);
            
            if (masterPassword.empty()) {
                std::cout << "Master password cannot be empty.\n";
                return;
            }
            
            if (passwordManager.initialize(masterPassword)) {
                std::cout << "Password Manager initialized successfully.\n";
                initialized = true;
            } else {
                std::cout << "Failed to initialize Password Manager.\n";
                return;
            }
        } else {
            // Load the master password hash and salt before authentication
            if (masterPasswordExists && !initialized) {
                passwordManager.load();
                initialized = true;
            }
            
            std::cout << "Enter master password: ";
            std::string masterPassword;
            std::getline(std::cin, masterPassword);
            
            if (!passwordManager.authenticate(masterPassword)) {
                std::cout << "Authentication failed. Incorrect master password.\n";
                return;
            }
            
            std::cout << "Authentication successful.\n";
        }
        
        // Password manager command loop
        bool running = true;
        while (running) {
            std::cout << "\nPassword Manager Commands:\n";
            std::cout << "1. Add password\n";
            std::cout << "2. Get password\n";
            std::cout << "3. List services\n";
            std::cout << "4. Remove password\n";
            std::cout << "5. Update password\n";
            std::cout << "6. Generate password\n";
            std::cout << "7. Change master password\n";
            std::cout << "8. Exit password manager\n";
            std::cout << "Enter choice: ";
            
            std::string choice;
            std::getline(std::cin, choice);
            
            if (choice == "1") { // Add password
                std::string service, username, password;
                std::cout << "Enter service name: ";
                std::getline(std::cin, service);
                std::cout << "Enter username: ";
                std::getline(std::cin, username);
                std::cout << "Enter password (or leave empty to generate): ";
                std::getline(std::cin, password);
                
                if (password.empty()) {
                    password = passwordManager.generatePassword();
                    std::cout << "Generated password: " << password << "\n";
                }
                
                if (passwordManager.addEntry(service, username, password)) {
                    std::cout << "Password added successfully.\n";
                } else {
                    std::cout << "Failed to add password.\n";
                }
            } else if (choice == "2") { // Get password
                std::string service;
                std::cout << "Enter service name: ";
                std::getline(std::cin, service);
                
                auto entry = passwordManager.getEntry(service);
                if (entry.service.empty()) {
                    std::cout << "Service not found.\n";
                } else {
                    std::cout << "Service: " << entry.service << "\n";
                    std::cout << "Username: " << entry.username << "\n";
                    // Display the actual password instead of a message about hash
                    std::cout << "Password: " << passwordManager.getPassword(service) << "\n";
                }
            } else if (choice == "3") { // List services
                auto services = passwordManager.listServices();
                if (services.empty()) {
                    std::cout << "No services stored.\n";
                } else {
                    std::cout << "Stored services:\n";
                    for (const auto& service : services) {
                        std::cout << "- " << service << "\n";
                    }
                }
            } else if (choice == "4") { // Remove password
                std::string service;
                std::cout << "Enter service name to remove: ";
                std::getline(std::cin, service);
                
                if (passwordManager.removeEntry(service)) {
                    std::cout << "Password removed successfully.\n";
                } else {
                    std::cout << "Failed to remove password. Service not found.\n";
                }
            } else if (choice == "5") { // Update password
                std::string service, username, password;
                std::cout << "Enter service name: ";
                std::getline(std::cin, service);
                std::cout << "Enter new username: ";
                std::getline(std::cin, username);
                std::cout << "Enter new password (or leave empty to generate): ";
                std::getline(std::cin, password);
                
                if (password.empty()) {
                    password = passwordManager.generatePassword();
                    std::cout << "Generated password: " << password << "\n";
                }
                
                if (passwordManager.updateEntry(service, username, password)) {
                    std::cout << "Password updated successfully.\n";
                } else {
                    std::cout << "Failed to update password. Service not found.\n";
                }
            } else if (choice == "6") { // Generate password
                std::string lengthStr;
                std::cout << "Enter password length (default 16): ";
                std::getline(std::cin, lengthStr);
                
                size_t length = 16;
                if (!lengthStr.empty()) {
                    try {
                        length = std::stoul(lengthStr);
                    } catch (...) {
                        std::cout << "Invalid length, using default (16).\n";
                    }
                }
                
                std::string password = passwordManager.generatePassword(length);
                std::cout << "Generated password: " << password << "\n";
            } else if (choice == "7") { // Change master password
                std::string oldPassword, newPassword;
                std::cout << "Enter current master password: ";
                std::getline(std::cin, oldPassword);
                std::cout << "Enter new master password: ";
                std::getline(std::cin, newPassword);
                
                if (passwordManager.changeMasterPassword(oldPassword, newPassword)) {
                    std::cout << "Master password changed successfully.\n";
                } else {
                    std::cout << "Failed to change master password. Incorrect current password.\n";
                }
            } else if (choice == "8") { // Exit
                running = false;
                std::cout << "Exiting password manager.\n";
            } else {
                std::cout << "Invalid choice. Please try again.\n";
            }
        }
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