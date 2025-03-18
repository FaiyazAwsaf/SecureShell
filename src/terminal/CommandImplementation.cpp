#include "terminal/CommandImplementation.h"

#include <windows.h>
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

        if (std::filesystem::is_directory(dest_path)) {
            dest_path /= source_path.filename();
        }

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
        std::cout << "Usage: dcreate <directory_name>\n";
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
        std::cout << "Usage: fcreate <filename>\n";
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

    // Prevent self-overwriting of encrypted files
void CommandImplementation::encrypt(const std::vector<std::string>& args) {
    if (args.size() != 3) {
        std::cout << "Usage: encrypt <input_file> <output_file> <password>\n";
        return;
    }

    std::string inputFile = args[0];
    std::string outputFile = args[1];
    std::string password = args[2];

    if (!std::filesystem::exists(inputFile)) {
        std::cout << "Error: Input file '" << inputFile << "' does not exist.\n";
        return;
    }

    if (std::filesystem::exists(outputFile)) {
        std::cout << "Warning: Output file '" << outputFile << "' already exists. Overwrite? (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear the newline

        if (tolower(choice) != 'y') {
            std::cout << "Encryption cancelled.\n";
            return;
        }
    }

    FileEncryption fileEncryptor;
    if (fileEncryptor.encryptFile(inputFile, outputFile, password)) {
        std::cout << "File encrypted successfully and saved to '" << outputFile << "'.\n";
    } else {
        std::cout << "Failed to encrypt the file.\n";
    }
}

    // Prevent self-overwriting of encrypted files
void CommandImplementation::decrypt(const std::vector<std::string>& args) {
    if (args.size() != 3) {
        std::cout << "Usage: decrypt <input_file> <output_file> <password>\n";
        return;
    }

    std::string inputFile = args[0];
    std::string outputFile = args[1];
    std::string password = args[2];

    if (!std::filesystem::exists(inputFile)) {
        std::cout << "Error: Input file '" << inputFile << "' does not exist.\n";
        return;
    }

    if (std::filesystem::exists(outputFile)) {
        std::cout << "Warning: Output file '" << outputFile << "' already exists. Overwrite? (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear the newline

        if (tolower(choice) != 'y') {
            std::cout << "Decryption cancelled.\n";
            return;
        }
    }

    FileEncryption fileEncryptor;
    
    if (!fileEncryptor.isFileEncrypted(inputFile)) {
        std::cout << "Failed to decrypt the file: The file does not appear to be encrypted.\n";
        return;
    }
    
    if (fileEncryptor.decryptFile(inputFile, outputFile, password)) {
        std::cout << "File decrypted successfully and saved to '" << outputFile << "'.\n";
    } else {
        std::cout << "File Decryption Unsuccessful: Incorrect Password\n";
    }
}

void CommandImplementation::compileAndRun(const std::string& filename) {
    std::string ext = Utils::getFileExtension(filename);
    bool autoExecute = true;
    std::string command;
    std::string outfile;
    int result = -1;

    if (ext == ".cpp" || ext == ".cc") {
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
        std::string compiler = "javac";
        command = compiler + " " + filename;
        std::cout << "Compiling Java file: " << command << "\n";
        result = system(command.c_str());
        
        if (result == 0 && autoExecute) {
            std::string className = filename.substr(0, filename.length() - ext.length());
            command = "java " + className;
            std::cout << "Running Java class: " << command << "\n";
            system(command.c_str());
            return; // Return early as we've already executed
        }
    }
    else if (ext == ".py") {
        command = "python " + filename;
        std::cout << "Running Python script: " << command << "\n";
        system(command.c_str());
        return; // Return early as we've already executed
    }
    else if (ext == ".js") {
        command = "node " + filename;
        std::cout << "Running JavaScript file: " << command << "\n";
        system(command.c_str());
        return; // Return early as we've already executed
    }
    else if (ext == ".rs") {
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
        std::cout << "Supported extensions: .cpp, .cc, .c, .java, .py, .js, .rs\n";
        return;
    }

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
    
    bool masterPasswordExists = passwordManager.hasMasterPassword();
    
    if (!masterPasswordExists && !initialized) {
        std::cout << "Password Manager - First Time Setup\n";
        std::cout << "\nPlease create a master password: ";
        std::string masterPassword = Utils::readMaskedPassword();
        
        if (masterPassword.empty()) {
            std::cout << "Master password cannot be empty.\n";
            return;
        }
        
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
        if (masterPasswordExists && !initialized) {
            passwordManager.load();
            initialized = true;
        }

        std::cout << "\n====  Password Manager  =====\n\n";
        std::cout << "Enter master password: ";
        std::string masterPassword = Utils::readMaskedPassword();
        
        if (!passwordManager.authenticate(masterPassword)) {
            std::cout << "\nAuthentication failed. Incorrect master password.\n";
            return;
        }
        
        std::cout << "\n***  Authentication successful ***\n\n";
        std::cout << "***  Welcome to Password Manager ***\n";

    }
    
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
        std::cout << "\nEnter choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") { //
            std::cout << "\n---- Add password ----\n\n";
            std::string service, username, password;
            std::cout << "Enter service name: ";
            std::getline(std::cin, service);
            std::cout << "Enter username: ";
            std::getline(std::cin, username);
            std::cout << "Enter password (or leave empty to generate): ";
            std::getline(std::cin, password);
            
            if (password.empty()) {
                password = passwordManager.generatePassword();
                std::cout << "\nGenerated password: " << password << "\n";
            }
            
            if (passwordManager.addEntry(service, username, password)) {
                std::cout << "\nPassword added successfully.\n";
            } else {
                std::cout << "\nFailed to add password.\n";
            }
        } else if (choice == "2") {
            std::cout << "\n---- Get password ----\n\n";
            std::string service;
            std::cout << "Enter service name: ";
            std::getline(std::cin, service);
            
            auto entry = passwordManager.getEntry(service);
            if (entry.service.empty()) {
                std::cout << "Service not found.\n";
            } else {
                std::cout << "\nService: " << entry.service << "\n";
                std::cout << "Username: " << entry.username << "\n";
                std::cout << "Password: " << passwordManager.getPassword(service) << "\n";
            }
        } else if (choice == "3") { // List services
            std::cout << "\n---- All Stored Services ----.\n";

            auto services = passwordManager.listServices();
            if (services.empty()) {
                std::cout << "\nNo services stored.\n";
            } else {
                for (const auto& service : services) {
                    std::cout << "# " << service << "\n";
                }
            }
        } else if (choice == "4") { // Remove password
            std::cout << "\n---- Remove Service ----\n\n";
            std::string service;
            std::cout << "\nEnter service name to remove: ";
            std::getline(std::cin, service);
            
            if (passwordManager.removeEntry(service)) {
                std::cout << "\nPassword removed successfully.\n";
            } else {
                std::cout << "\nFailed to remove password. Service not found.\n";
            }
        } else if (choice == "5") {
            std::cout << "\n---- Update Service ----\n\n";

            std::string service, username, password;
            std::cout << "\nEnter service name: ";
            std::getline(std::cin, service);
            std::cout << "Enter new username: ";
            std::getline(std::cin, username);
            std::cout << "Enter new password (or leave empty to generate): ";
            std::getline(std::cin, password);
            
            if (password.empty()) {
                password = passwordManager.generatePassword();
                std::cout << "\nGenerated password: " << password << "\n";
            }
            
            if (passwordManager.updateEntry(service, username, password)) {
                std::cout << "\nPassword updated successfully.\n";
            } else {
                std::cout << "\nFailed to update password. Service not found.\n";
            }
        } else if (choice == "6") {
            std::cout << "\n---- Generate Password ----\n\n";
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
            std::cout << "\nEnter current master password: ";
            oldPassword = Utils::readMaskedPassword();
            std::cout << "\nEnter new master password: ";
            newPassword = Utils::readMaskedPassword();
            
            if (!Utils::validatePasswordStrength(newPassword)) {
                std::cout << "\nPassword is too weak.\nIt must be at least 8 characters long and contain letters, special characters and numbers.\n";
                continue;
            }
            
            if (passwordManager.changeMasterPassword(oldPassword, newPassword)) {
                std::cout << "\nMaster password changed successfully.\n";
            } else {
                std::cout << "\nFailed to change master password. Incorrect current password.\n";
            }
        } else if (choice == "8") { // Exit
            running = false;
            std::cout << "Exiting password manager.\n";
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void CommandImplementation::cat(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: cat <filename>\n";
        return;
    }

    std::ifstream file(args[0]);
    if (!file) {
        std::cout << "Error: Cannot open file '" << args[0] << "'\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << '\n';
    }
}

void CommandImplementation::grep(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: grep <pattern> <filename>\n";
        return;
    }

    std::string pattern = args[0];
    std::string filename = args[1];
    std::ifstream file(filename);

    if (!file) {
        std::cout << "Error: Cannot open file '" << filename << "'\n";
        return;
    }

    std::string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        lineNum++;
        if (line.find(pattern) != std::string::npos) {
            std::cout << lineNum << ": " << line << '\n';
        }
    }
}

void CommandImplementation::head(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: head <filename> [number_of_lines]\n";
        return;
    }

    std::string filename = args[0];
    int numLines = args.size() > 1 ? std::stoi(args[1]) : 10;

    std::ifstream file(filename);
    if (!file) {
        std::cout << "Error: Cannot open file '" << filename << "'\n";
        return;
    }

    std::string line;
    int count = 0;
    while (std::getline(file, line) && count < numLines) {
        std::cout << line << '\n';
        count++;
    }
}

void CommandImplementation::tree(const std::vector<std::string>& args) {
    std::string path = args.empty() ? "." : args[0];

    std::function<void(const std::filesystem::path&, std::string)> printTree;
    printTree = [&](const std::filesystem::path& path, std::string prefix) {
        try {
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                std::cout << prefix << "|-- " << entry.path().filename().string() << '\n';
                if (entry.is_directory()) {
                    printTree(entry.path(), prefix + "|   ");
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cout << "Error accessing directory: " << e.what() << '\n';
        }
    };

    std::cout << path << '\n';
    printTree(path, "");
}

void CommandImplementation::find(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: find <pattern>\n";
        return;
    }

    std::string pattern = args[0];
    std::function<void(const std::filesystem::path&)> searchFiles;

    searchFiles = [&](const std::filesystem::path& path) {
        try {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
                if (entry.path().filename().string().find(pattern) != std::string::npos) {
                    std::cout << entry.path().string() << '\n';
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cout << "Error searching files: " << e.what() << '\n';
        }
    };

    searchFiles(".");
}

void CommandImplementation::system_info(const std::vector<std::string>& args) {
    #ifdef _WIN32
        system("systeminfo");
    #else
        system("uname -a");
        std::cout << "\nCPU Info:\n";
        system("cat /proc/cpuinfo | grep 'model name' | uniq");
        std::cout << "\nMemory Info:\n";
        system("free -h");
    #endif
}

void CommandImplementation::stat(const std::vector<std::string>& args){
    if (args.empty()) {
        std::cout << "Usage: stat <filename/directory>\n";
        return;
    }

    std::filesystem::path path = args[0];
    try {
        if (!std::filesystem::exists(path)) {
            std::cout << "Error: '" << path.string() << "' does not exist.\n";
            return;
        }

        std::filesystem::file_status status = std::filesystem::status(path);
        auto lastWriteTime = std::filesystem::last_write_time(path);
        auto fileSize = std::filesystem::is_regular_file(path) ? std::filesystem::file_size(path) : 0;

        std::cout << "  File: " << path.filename().string() << "\n";
        std::cout << "  Path: " << std::filesystem::absolute(path).string() << "\n";
        std::cout << "  Size: " << fileSize << " bytes\n";

        std::cout << "  Type: ";
        if (std::filesystem::is_regular_file(path)) std::cout << "Regular file\n";
        else if (std::filesystem::is_directory(path)) std::cout << "Directory\n";
        else if (std::filesystem::is_symlink(path)) std::cout << "Symbolic link\n";
        else std::cout << "Other\n";

        std::cout << "  Permissions: ";
        auto perms = status.permissions();
        std::cout << ((perms & std::filesystem::perms::owner_read) != std::filesystem::perms::none ? "r" : "-");
        std::cout << ((perms & std::filesystem::perms::owner_write) != std::filesystem::perms::none ? "w" : "-");
        std::cout << ((perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ? "x" : "-");
        std::cout << ((perms & std::filesystem::perms::group_read) != std::filesystem::perms::none ? "r" : "-");
        std::cout << ((perms & std::filesystem::perms::group_write) != std::filesystem::perms::none ? "w" : "-");
        std::cout << ((perms & std::filesystem::perms::group_exec) != std::filesystem::perms::none ? "x" : "-");
        std::cout << ((perms & std::filesystem::perms::others_read) != std::filesystem::perms::none ? "r" : "-");
        std::cout << ((perms & std::filesystem::perms::others_write) != std::filesystem::perms::none ? "w" : "-");
        std::cout << ((perms & std::filesystem::perms::others_exec) != std::filesystem::perms::none ? "x" : "-");
        std::cout << "\n";

    } catch (const std::filesystem::filesystem_error& e) {
        std::cout << "Error getting file properties: " << e.what() << "\n";
    }
}
