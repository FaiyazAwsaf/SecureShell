#pragma once

#include <string>

namespace passman {

/**
 * @struct PasswordEntry
 * @brief Structure to hold password entry information
 */
struct PasswordEntry {
    std::string service;
    std::string username;
    std::string encryptedPassword;
    std::string serviceLink;
    std::string salt;
};

} // namespace passman