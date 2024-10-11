#include "GenerateRandomPassword.h"
#include <random>
#include <string>

std::string Utils::generateRandomPassword(size_t length) {
    const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+";
    std::string password;
    password.reserve(length); // Reserve space for performance

    std::random_device rd; // Non-deterministic generator
    std::mt19937 generator(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<> distribution(0, charset.size() - 1);

    for (size_t i = 0; i < length; ++i) {
        password += charset[distribution(generator)];
    }

    return password;
}
