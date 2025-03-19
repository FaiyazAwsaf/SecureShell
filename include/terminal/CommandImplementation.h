#ifndef SECURESHELL_COMMANDIMPLEMENTATION_H
#define SECURESHELL_COMMANDIMPLEMENTATION_H

#include "terminal/Terminal.h"
#include "utils/Utils.h"
#include <string>
#include <vector>

class FileOperations;
class PasswordManagerOperations;

class CommandImplementation {
public:
    explicit CommandImplementation(Terminal& terminal);
    ~CommandImplementation();
    
    void help() const;
    void exit();
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
    void compile(const std::vector<std::string>& args);
    void passman(const std::vector<std::string>& args);
    void encrypt(const std::vector<std::string>& args);
    void decrypt(const std::vector<std::string>& args);
    void system_info(const std::vector<std::string>& args);

private:
    Terminal& terminal;
    FileOperations* fileOperations;
    PasswordManagerOperations* passwordOperations;
    void compileAndRun(const std::string& filename);
};


#endif //SECURESHELL_COMMANDIMPLEMENTATION_H