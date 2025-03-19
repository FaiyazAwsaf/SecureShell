#include "terminal/CommandImplementation.h"
#include "terminal/FileOperations.h"
#include "passman/PasswordManagerOperations.h"
#include "encryption/FileEncryption.h"

#include <windows.h>
#include <iostream>
#include <filesystem>

CommandImplementation::CommandImplementation(Terminal& terminal) 
    : terminal(terminal), 
      fileOperations(new FileOperations()),
      passwordOperations(new PasswordManagerOperations()) {}

CommandImplementation::~CommandImplementation() {
    delete fileOperations;
    delete passwordOperations;
}

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

// File operation methods delegated to FileOperations class
void CommandImplementation::cd(const std::vector<std::string>& args) { fileOperations->cd(args); }
void CommandImplementation::ls(const std::vector<std::string>& args) { fileOperations->ls(args); }
void CommandImplementation::copy(const std::vector<std::string>& args) { fileOperations->copy(args); }
void CommandImplementation::move(const std::vector<std::string>& args) { fileOperations->move(args); }
void CommandImplementation::rename(const std::vector<std::string>& args) { fileOperations->rename(args); }
void CommandImplementation::create_directory(const std::vector<std::string>& args) { fileOperations->create_directory(args); }
void CommandImplementation::create_file(const std::vector<std::string>& args) { fileOperations->create_file(args); }
void CommandImplementation::display_permission(const std::vector<std::string>& args) { fileOperations->display_permission(args); }
void CommandImplementation::get_current_directory(const std::vector<std::string>& args) { fileOperations->get_current_directory(args); }
void CommandImplementation::remove(const std::vector<std::string>& args) { fileOperations->remove(args); }

// Encryption methods have been implemented in the encrypt/decrypt methods at the top of the file

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

void CommandImplementation::system_info(const std::vector<std::string>& args) {
    std::cout << "\n==== System Information ====\n";
    
    #ifdef _WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    
    // CPU Information
    std::cout << "\nCPU Information:\n";
    std::cout << "  Processor Architecture: ";
    switch (sysInfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64: std::cout << "x64 (AMD or Intel)\n"; break;
        case PROCESSOR_ARCHITECTURE_ARM: std::cout << "ARM\n"; break;
        case PROCESSOR_ARCHITECTURE_IA64: std::cout << "Intel Itanium\n"; break;
        case PROCESSOR_ARCHITECTURE_INTEL: std::cout << "x86\n"; break;
        default: std::cout << "Unknown\n";
    }
    std::cout << "  Number of Processors: " << sysInfo.dwNumberOfProcessors << "\n";
    
    // Memory Information
    std::cout << "\nMemory Information:\n";
    std::cout << "  Total Physical Memory: " << memInfo.ullTotalPhys / (1024 * 1024) << " MB\n";
    std::cout << "  Available Physical Memory: " << memInfo.ullAvailPhys / (1024 * 1024) << " MB\n";
    std::cout << "  Memory Load: " << memInfo.dwMemoryLoad << "%\n";
    
    // OS Version Information
    OSVERSIONINFO osInfo;
    osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    
    // Note: GetVersionEx is deprecated, but still works for basic info
    #pragma warning(disable: 4996)
    GetVersionEx(&osInfo);
    #pragma warning(default: 4996)
    
    std::cout << "\nOperating System Information:\n";
    std::cout << "  Windows Version: " << osInfo.dwMajorVersion << "." << osInfo.dwMinorVersion << "\n";
    std::cout << "  Build Number: " << osInfo.dwBuildNumber << "\n";
    #else
    // Unix/Linux/Mac implementation would go here
    std::cout << "System information not available for this platform.\n";
    #endif
    
    std::cout << "\n==== End of System Information ====\n";
}

// Password manager operations delegated to PasswordManagerOperations class
void CommandImplementation::passman(const std::vector<std::string>& args) { passwordOperations->passman(args); }

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

// Removed duplicate system_info implementation

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
