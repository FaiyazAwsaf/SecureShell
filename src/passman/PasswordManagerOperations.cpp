#include "passman/PasswordManagerOperations.h"
#include <iostream>

PasswordManagerOperations::PasswordManagerOperations() : initialized(false) {}

void PasswordManagerOperations::passman(const std::vector<std::string>& args) {
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
        
        if (choice == "1") {
            addPassword();
        } else if (choice == "2") {
            getPassword();
        } else if (choice == "3") {
            listServices();
        } else if (choice == "4") {
            removePassword();
        } else if (choice == "5") {
            updatePassword();
        } else if (choice == "6") {
            generatePassword();
        } else if (choice == "7") {
            changeMasterPassword();
        } else if (choice == "8") {
            std::cout << "Exiting Password Manager...\n";
            running = false;
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void PasswordManagerOperations::addPassword() {
    std::cout << "\n---- Add password ----\n\n";
    std::string service, username, password;
    
    std::cout << "Enter service name: ";
    std::getline(std::cin, service);
    
    if (service.empty()) {
        std::cout << "Service name cannot be empty.\n";
        return;
    }
    
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    
    std::cout << "Enter password (or leave empty to generate): ";
    password = Utils::readMaskedPassword();
    
    if (password.empty()) {
        password = Utils::generateRandomString(12);
        std::cout << "Generated password: " << password << "\n";
    }
    
    if (passwordManager.addEntry(service, username, password)) {
        std::cout << "Password added successfully.\n";
    } else {
        std::cout << "Failed to add password.\n";
    }
}

void PasswordManagerOperations::getPassword() {
    std::cout << "\n---- Get password ----\n\n";
    std::string service;
    
    std::cout << "Enter service name: ";
    std::getline(std::cin, service);
    
    if (service.empty()) {
        std::cout << "Service name cannot be empty.\n";
        return;
    }
    
    auto entry = passwordManager.getEntry(service);
    
    if (entry.service.empty()) {
        std::cout << "No password found for service: " << service << "\n";
        return;
    }
    
    std::cout << "\nService: " << entry.service << "\n";
    std::cout << "Username: " << entry.username << "\n";
    std::cout << "Password: " << passwordManager.getPassword(entry.service) << "\n";
}

void PasswordManagerOperations::listServices() {
    std::cout << "\n---- Available services ----\n\n";
    
    auto services = passwordManager.listServices();
    
    if (services.empty()) {
        std::cout << "No passwords stored.\n";
        return;
    }
    
    for (const auto& service : services) {
        std::cout << "- " << service << "\n";
    }
}

void PasswordManagerOperations::removePassword() {
    std::cout << "\n---- Remove password ----\n\n";
    std::string service;
    
    std::cout << "Enter service name: ";
    std::getline(std::cin, service);
    
    if (service.empty()) {
        std::cout << "Service name cannot be empty.\n";
        return;
    }
    
    std::cout << "Are you sure you want to remove the password for '" << service << "'? (y/n): ";
    std::string confirmation;
    std::getline(std::cin, confirmation);
    
    if (confirmation != "y" && confirmation != "Y") {
        std::cout << "Operation cancelled.\n";
        return;
    }
    
    if (passwordManager.removeEntry(service)) {
        std::cout << "Password removed successfully.\n";
    } else {
        std::cout << "Failed to remove password. Service not found.\n";
    }
}

void PasswordManagerOperations::updatePassword() {
    std::cout << "\n---- Update password ----\n\n";
    std::string service, username, password;
    
    std::cout << "Enter service name: ";
    std::getline(std::cin, service);
    
    if (service.empty()) {
        std::cout << "Service name cannot be empty.\n";
        return;
    }
    
    auto entry = passwordManager.getEntry(service);
    
    if (entry.service.empty()) {
        std::cout << "No password found for service: " << service << "\n";
        return;
    }
    
    std::cout << "Current username: " << entry.username << "\n";
    std::cout << "Enter new username (leave empty to keep current): ";
    std::getline(std::cin, username);
    
    if (username.empty()) {
        username = entry.username;
    }
    
    std::cout << "Enter new password (leave empty to generate): ";
    password = Utils::readMaskedPassword();
    
    if (password.empty()) {
        password = Utils::generateRandomString(12);
        std::cout << "Generated password: " << password << "\n";
    }
    
    if (passwordManager.updateEntry(service, username, password)) {
        std::cout << "Password updated successfully.\n";
    } else {
        std::cout << "Failed to update password.\n";
    }
}

void PasswordManagerOperations::generatePassword() {
    std::cout << "\n---- Generate password ----\n\n";
    std::cout << "Enter desired password length (default is 12): ";
    std::string lengthStr;
    std::getline(std::cin, lengthStr);
    
    size_t length = 12;
    if (!lengthStr.empty()) {
        try {
            length = std::stoi(lengthStr);
            if (length < 8) {
                std::cout << "Password length too short. Using minimum length of 8.\n";
                length = 8;
            } else if (length > 64) {
                std::cout << "Password length too long. Using maximum length of 64.\n";
                length = 64;
            }
        } catch (const std::exception&) {
            std::cout << "Invalid input. Using default length of 12.\n";
        }
    }
    
    std::string password = Utils::generateRandomString(length);
    std::cout << "Generated password: " << password << "\n";
}

void PasswordManagerOperations::changeMasterPassword() {
    std::cout << "\n---- Change master password ----\n\n";
    std::cout << "Enter current master password: ";
    std::string currentPassword = Utils::readMaskedPassword();
    
    if (!passwordManager.authenticate(currentPassword)) {
        std::cout << "Authentication failed. Incorrect master password.\n";
        return;
    }
    
    std::cout << "Enter new master password: ";
    std::string newPassword = Utils::readMaskedPassword();
    
    if (newPassword.empty()) {
        std::cout << "New master password cannot be empty.\n";
        return;
    }
    
    if (!Utils::validatePasswordStrength(newPassword)) {
        std::cout << "Password is too weak.\nIt must be at least 8 characters long and contain letters, special characters and numbers.\n";
        return;
    }
    
    std::cout << "Confirm new master password: ";
    std::string confirmPassword = Utils::readMaskedPassword();
    
    if (newPassword != confirmPassword) {
        std::cout << "Passwords do not match.\n";
        return;
    }
    
    if (passwordManager.changeMasterPassword(currentPassword, newPassword)) {
        std::cout << "Master password changed successfully.\n";
    } else {
        std::cout << "Failed to change master password.\n";
    }
}