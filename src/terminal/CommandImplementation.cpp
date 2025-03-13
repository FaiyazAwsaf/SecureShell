#include "terminal/CommandImplementation.h"
#include <iostream>
#include <filesystem>

CommandImplementation::CommandImplementation(Terminal& terminal) : terminal(terminal) {}

void CommandImplementation::help() const {
    std::cout << "Available commands:\n";
    auto commands = terminal.getCommandParser().getCommandList();
    for (const auto& [cmd, desc] : commands) {
        std::cout << cmd << "\t" << desc << "\n";
    }
}

void CommandImplementation::exit() {
    terminal.stop();
}

void CommandImplementation::cd(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: cd <directory>\n";
        return;
    }
    if (!Utils::changeDirectory(args[0])) {
        std::cout << "Failed to change directory\n";
    }
}

void CommandImplementation::ls(const std::vector<std::string>& args) {
    std::string path = args.empty() ? "." : args[0];
    auto files = Utils::listDirectory(path);
    for (const auto& file : files) {
        std::cout << file << "\n";
    }
}

void CommandImplementation::copy(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        std::cout << "Usage: copy <source> <destination>\n";
        return;
    }

    std::filesystem::path source_path = args[0];
    std::filesystem::path dest_path = args[1];

    try {
        std::filesystem::copy(source_path, dest_path, 
            std::filesystem::copy_options::overwrite_existing);
        std::cout << "File copied successfully.\n";
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cout << "Error copying the file: " << e.what() << "\n";
    }
}

void CommandImplementation::rename(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        std::cout << "Usage: rename <old_name> <new_name>\n";
        return;
    }

    std::filesystem::path old_path = args[0];
    std::filesystem::path new_path = args[1];

    try {
        if (!std::filesystem::exists(old_path)) {
            std::cout << "Error: File '" << old_path.string() << "' does not exist.\n";
            return;
        }

        if (std::filesystem::exists(new_path)) {
            std::cout << "Error: File '" << new_path.string() << "' already exists.\n";
            return;
        }

        std::filesystem::rename(old_path, new_path);
        std::cout << "Successfully renamed '" << old_path.string() << "' to '" << new_path.string() << "'.\n";
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cout << "Error renaming file: " << e.what() << "\n";
    }
}

void CommandImplementation::move(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        std::cout << "Usage: move <source> <destination>\n";
        return;
    }

    std::filesystem::path source_path = args[0];
    std::filesystem::path dest_path = args[1];

    try {
        if (!std::filesystem::exists(source_path)) {
            std::cout << "Error: Source '" << source_path.string() << "' does not exist.\n";
            return;
        }

        // If destination is a directory, append the source filename
        if (std::filesystem::is_directory(dest_path)) {
            dest_path /= source_path.filename();
        }

        // Check if destination already exists
        if (std::filesystem::exists(dest_path)) {
            std::cout << "Warning: Destination '" << dest_path.string() << "' already exists. Overwrite? (y/n): ";
            char choice;
            std::cin >> choice;
            std::cin.ignore(); // Clear the newline

            if (tolower(choice) != 'y') {
                std::cout << "Move operation cancelled.\n";
                return;
            }
        }

        std::filesystem::rename(source_path, dest_path);
        std::cout << "Successfully moved '" << source_path.string() << "' to '" << dest_path.string() << "'.\n";
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cout << "Error moving file: " << e.what() << "\n";
        
        // If rename fails (e.g., across devices), try copy and delete
        try {
            std::filesystem::copy(source_path, dest_path, 
                std::filesystem::copy_options::overwrite_existing | 
                std::filesystem::copy_options::recursive);
            std::filesystem::remove_all(source_path);
            std::cout << "Successfully moved using copy and delete method.\n";
        }
        catch (const std::filesystem::filesystem_error& e2) {
            std::cout << "Error during fallback copy-delete: " << e2.what() << "\n";
        }
    }
}

void CommandImplementation::create_directory(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: mkdir <directory_name>\n";
        return;
    }

    std::filesystem::path dir_path = args[0];

    try {
        if (std::filesystem::exists(dir_path)) {
            std::cout << "Error: Directory '" << dir_path.string() << "' already exists.\n";
            return;
        }

        if (std::filesystem::create_directory(dir_path)) {
            std::cout << "Directory '" << dir_path.string() << "' created successfully.\n";
        } else {
            std::cout << "Failed to create directory.\n";
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cout << "Error creating directory: " << e.what() << "\n";
    }
}

void CommandImplementation::create_file(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: touch <filename>\n";
        return;
    }

    std::filesystem::path file_path = args[0];

    try {
        if (std::filesystem::exists(file_path)) {
            std::cout << "Error: File '" << file_path.string() << "' already exists.\n";
            return;
        }

        std::ofstream file(file_path);
        if (file.is_open()) {
            file.close();
            std::cout << "File '" << file_path.string() << "' created successfully.\n";
        } else {
            std::cout << "Failed to create file.\n";
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cout << "Error creating file: " << e.what() << "\n";
    }
}

void CommandImplementation::display_permission(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: permission <filename>\n";
        return;
    }

    std::filesystem::path file_path = args[0];

    try {
        if (!std::filesystem::exists(file_path)) {
            std::cout << "Error: File '" << file_path.string() << "' does not exist.\n";
            return;
        }

        std::filesystem::perms permissions = std::filesystem::status(file_path).permissions();

        std::filesystem::perms permission_collection[] = {
            std::filesystem::perms::owner_read, std::filesystem::perms::owner_write, std::filesystem::perms::owner_exec,
            std::filesystem::perms::group_read, std::filesystem::perms::group_write, std::filesystem::perms::group_exec,
            std::filesystem::perms::others_read, std::filesystem::perms::others_write, std::filesystem::perms::others_exec
        };

        std::string permission_keywords = "rwxrwxrwx";
        std::cout << "Permissions for '" << file_path.string() << "': ";
        
        for (int i = 0; i < 9; i++) {
            std::cout << ((permissions & permission_collection[i]) != std::filesystem::perms::none ? 
                         permission_keywords[i] : '-');
        }
        std::cout << "\n";
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cout << "Error displaying permissions: " << e.what() << "\n";
    }
}

void CommandImplementation::get_current_directory(const std::vector<std::string>& args) {
    try {
        std::cout << "Current working directory: " << std::filesystem::current_path().string() << std::endl;
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cout << "Error getting current directory: " << e.what() << "\n";
    }
}

void CommandImplementation::remove(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: del <filename/directory>\n";
        return;
    }

    std::filesystem::path path = args[0];

    try {
        if (!std::filesystem::exists(path)) {
            std::cout << "Error: '" << path.string() << "' does not exist.\n";
            return;
        }

        std::cout << "Are you sure you want to delete '" << path.string() << "'? (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore(); 

        if (tolower(choice) != 'y') {
            std::cout << "Delete operation cancelled.\n";
            return;
        }

        if (std::filesystem::remove_all(path)) {
            std::cout << "Successfully deleted '" << path.string() << "'.\n";
        } else {
            std::cout << "Failed to delete item.\n";
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cout << "Error deleting item: " << e.what() << "\n";
    }
}

void CommandImplementation::compileAndRun(const std::string& filename) {
    std::string ext = Utils::getFileExtension(filename);
    bool autoExecute = true;
    std::string command;
    std::string outfile;
    int result = -1;

    // Determine compilation/execution based on file extension
    if (ext == ".cpp" || ext == ".cc") {
        // C++ compilation
        std::string compiler = "g++";
        outfile = filename.substr(0, filename.length() - ext.length());
        #ifdef _WIN32
        outfile += ".exe";
        #endif
        
        command = compiler + " " + filename + " -o " + outfile;
        std::cout << "Compiling C++ file: " << command << "\n";
        result = system(command.c_str());
    } 
    else if (ext == ".c") {
        // C compilation
        std::string compiler = "gcc";
        outfile = filename.substr(0, filename.length() - ext.length());
        #ifdef _WIN32
        outfile += ".exe";
        #endif
        
        command = compiler + " " + filename + " -o " + outfile;
        std::cout << "Compiling C file: " << command << "\n";
        result = system(command.c_str());
    }
    else if (ext == ".java") {
        // Java compilation
        std::string compiler = "javac";
        command = compiler + " " + filename;
        std::cout << "Compiling Java file: " << command << "\n";
        result = system(command.c_str());
        
        if (result == 0 && autoExecute) {
            // Extract class name (assuming filename matches class name)
            std::string className = filename.substr(0, filename.length() - ext.length());
            command = "java " + className;
            std::cout << "Running Java class: " << command << "\n";
            system(command.c_str());
            return; // Return early as we've already executed
        }
    }
    else if (ext == ".py") {
        // Python execution (no compilation needed)
        command = "python " + filename;
        std::cout << "Running Python script: " << command << "\n";
        system(command.c_str());
        return; // Return early as we've already executed
    }
    else if (ext == ".rs") {
        // Rust compilation
        std::string compiler = "rustc";
        outfile = filename.substr(0, filename.length() - ext.length());
        #ifdef _WIN32
        outfile += ".exe";
        #endif
        
        command = compiler + " " + filename + " -o " + outfile;
        std::cout << "Compiling Rust file: " << command << "\n";
        result = system(command.c_str());
    }
    else {
        std::cout << "Unsupported file extension: " << ext << "\n";
        std::cout << "Supported extensions: .cpp, .cc, .c, .java, .py, .rs\n";
        return;
    }

    // Auto-execute compiled languages if compilation was successful
    if (result == 0 && autoExecute && !outfile.empty()) {
        std::cout << "Executing: " << outfile << "\n";
        system(outfile.c_str());
    } else if (result != 0) {
        std::cout << "Compilation failed with error code: " << result << "\n";
    }
}

void CommandImplementation::compile(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: compile <filename>\n";
        return;
    }
    compileAndRun(args[0]);
}

void CommandImplementation::passman(const std::vector<std::string>& args) {
    static passman::PasswordManager passwordManager;
    static bool initialized = false;
    
    // Check if master password file exists
    bool masterPasswordExists = passwordManager.hasMasterPassword();
    
    if (!masterPasswordExists && !initialized) {
        std::cout << "Password Manager - First Time Setup\n";
        std::cout << "Please create a master password: ";
        std::string masterPassword = Utils::readMaskedPassword();
        
        if (masterPassword.empty()) {
            std::cout << "Master password cannot be empty.\n";
            return;
        }
        
        // Validate password strength
        if (!Utils::validatePasswordStrength(masterPassword)) {
            std::cout << "Password is too weak.\nIt must be at least 8 characters long and contain letters, special characters and numbers.\n";
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
        std::string masterPassword = Utils::readMaskedPassword();
        
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
            oldPassword = Utils::readMaskedPassword();
            std::cout << "Enter new master password: ";
            newPassword = Utils::readMaskedPassword();
            
            // Validate new password strength
            if (!Utils::validatePasswordStrength(newPassword)) {
                std::cout << "Password is too weak.\nIt must be at least 8 characters long and contain letters, special characters and numbers.\n";
                continue;
            }
            
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
}
