#pragma once

#include "utils/Utils.h"
#include <string>
#include <vector>
#include <filesystem>

class FileOperations {
public:
    FileOperations();
    
    void cd(const std::vector<std::string>& args);
    void ls(const std::vector<std::string>& args);
    void copy(const std::vector<std::string>& args);
    void move(const std::vector<std::string>& args);
    void rename(const std::vector<std::string>& args);
    void create_directory(const std::vector<std::string>& args);
    void create_file(const std::vector<std::string>& args);
    void display_permission(const std::vector<std::string>& args);
    void get_current_directory(const std::vector<std::string>& args);
    void remove(const std::vector<std::string>& args);
    void cat(const std::vector<std::string>& args);
    void write(const std::vector<std::string>& args);
    void grep(const std::vector<std::string>& args);
    void head(const std::vector<std::string>& args);
    void tree(const std::vector<std::string>& args);
    void find(const std::vector<std::string>& args);
    void stat(const std::vector<std::string>& args);

private:
    void printDirectoryTree(const std::string& path, const std::string& prefix, int maxDepth, int currentDepth);
    void findFiles(const std::string& directory, const std::string& pattern);
};