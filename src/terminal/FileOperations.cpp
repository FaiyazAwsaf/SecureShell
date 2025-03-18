#include "terminal/FileOperations.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>

FileOperations::FileOperations() {}

void FileOperations::cd(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: cd <directory>\n";
        return;
    }
    if (!Utils::changeDirectory(args[0])) {
        std::cout << "Failed to change directory\n";
    }
}

void FileOperations::ls(const std::vector<std::string>& args) {
    std::string path = args.empty() ? "." : args[0];
    auto files = Utils::listDirectory(path);
    for (const auto& file : files) {
        std::cout << file << "\n";
    }
}

void FileOperations::copy(const std::vector<std::string>& args) {
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

void FileOperations::rename(const std::vector<std::string>& args) {
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

void FileOperations::move(const std::vector<std::string>& args) {
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
            std::cin.ignore();

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

void FileOperations::create_directory(const std::vector<std::string>& args) {
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

void FileOperations::create_file(const std::vector<std::string>& args) {
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

void FileOperations::display_permission(const std::vector<std::string>& args) {
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

void FileOperations::get_current_directory(const std::vector<std::string>& args) {
    try {
        std::cout << "Current working directory: " << std::filesystem::current_path().string() << std::endl;
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cout << "Error getting current directory: " << e.what() << "\n";
    }
}

void FileOperations::remove(const std::vector<std::string>& args) {
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

void FileOperations::cat(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: cat <filename>\n";
        return;
    }

    std::string filename = args[0];
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Error: Could not open file '" << filename << "'\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << "\n";
    }

    file.close();
}

void FileOperations::grep(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: grep <pattern> <filename>\n";
        return;
    }

    std::string pattern = args[0];
    std::string filename = args[1];
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Error: Could not open file '" << filename << "'\n";
        return;
    }

    std::string line;
    int lineNumber = 0;
    bool found = false;

    while (std::getline(file, line)) {
        lineNumber++;
        if (line.find(pattern) != std::string::npos) {
            std::cout << lineNumber << ": " << line << "\n";
            found = true;
        }
    }

    if (!found) {
        std::cout << "Pattern '" << pattern << "' not found in file '" << filename << "'\n";
    }

    file.close();
}

void FileOperations::head(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: head <filename> [lines]\n";
        return;
    }

    std::string filename = args[0];
    int numLines = 10;

    if (args.size() > 1) {
        try {
            numLines = std::stoi(args[1]);
        } catch (const std::exception&) {
            std::cout << "Invalid number of lines. Using default (10).\n";
        }
    }

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Error: Could not open file '" << filename << "'\n";
        return;
    }

    std::string line;
    int lineCount = 0;

    while (std::getline(file, line) && lineCount < numLines) {
        std::cout << line << "\n";
        lineCount++;
    }

    file.close();
}

void FileOperations::tree(const std::vector<std::string>& args) {
    std::string path = args.empty() ? "." : args[0];
    int maxDepth = args.size() > 1 ? std::stoi(args[1]) : -1;
    
    try {
        if (!std::filesystem::exists(path)) {
            std::cout << "Error: Path '" << path << "' does not exist.\n";
            return;
        }
        
        std::cout << path << "\n";
        printDirectoryTree(path, "", maxDepth, 0);
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

void FileOperations::printDirectoryTree(const std::string& path, const std::string& prefix, int maxDepth, int currentDepth) {
    if (maxDepth != -1 && currentDepth >= maxDepth) {
        return;
    }
    
    std::vector<std::filesystem::directory_entry> entries;
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            entries.push_back(entry);
        }
        
        std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
            return a.path().filename().string() < b.path().filename().string();
        });
        
        for (size_t i = 0; i < entries.size(); ++i) {
            const auto& entry = entries[i];
            bool isLast = (i == entries.size() - 1);
            
            std::string connector = isLast ? "└── " : "├── ";
            std::string nextPrefix = isLast ? "    " : "│   ";
            
            std::cout << prefix << connector << entry.path().filename().string() << "\n";
            
            if (std::filesystem::is_directory(entry.path())) {
                printDirectoryTree(entry.path().string(), prefix + nextPrefix, maxDepth, currentDepth + 1);
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Error accessing directory: " << e.what() << "\n";
    }
}

void FileOperations::find(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: find <directory> <pattern>\n";
        return;
    }
    
    std::string directory = args[0];
    std::string pattern = args[1];
    
    if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory)) {
        std::cout << "Error: '" << directory << "' is not a valid directory.\n";
        return;
    }
    
    std::cout << "Searching for files matching '" << pattern << "' in '" << directory << "'...\n";
    findFiles(directory, pattern);
}

void FileOperations::findFiles(const std::string& directory, const std::string& pattern) {
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
            std::string filename = entry.path().filename().string();
            if (filename.find(pattern) != std::string::npos) {
                std::cout << entry.path().string() << "\n";
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Error during search: " << e.what() << "\n";
    }
}

void FileOperations::stat(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: stat <filename>\n";
        return;
    }
    
    std::string filename = args[0];
    
    try {
        if (!std::filesystem::exists(filename)) {
            std::cout << "Error: File '" << filename << "' does not exist.\n";
            return;
        }
        
        auto fileStatus = std::filesystem::status(filename);
        auto lastWriteTime = std::filesystem::last_write_time(filename);
        auto fileSize = std::filesystem::is_regular_file(filename) ? std::filesystem::file_size(filename) : 0;
        
        std::cout << "File: " << filename << "\n";
        std::cout << "Size: " << fileSize << " bytes\n";
        std::cout << "Type: " << (std::filesystem::is_directory(filename) ? "Directory" : 
                                 std::filesystem::is_regular_file(filename) ? "Regular File" : "Other") << "\n";

        std::cout << "Permissions: ";
        display_permission(std::vector<std::string>{filename});
    } catch (const std::exception& e) {
        std::cout << "Error getting file stats: " << e.what() << "\n";
    }
}
