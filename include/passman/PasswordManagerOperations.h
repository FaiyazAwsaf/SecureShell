#pragma once

#include "passman/PasswordManager.h"
#include "utils/Utils.h"
#include <string>
#include <vector>

class PasswordManagerOperations {
public:
    PasswordManagerOperations();
    ~PasswordManagerOperations() = default;

    void passman(const std::vector<std::string>& args);

private:
    void addPassword();
    void getPassword();
    void listServices();
    void removePassword();
    void updatePassword();
    void generatePassword();
    void changeMasterPassword();

    passman::PasswordManager passwordManager;
    bool initialized;
};