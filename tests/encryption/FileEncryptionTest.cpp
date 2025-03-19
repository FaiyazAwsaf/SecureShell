#include "encryption/FileEncryption.h"
#include "../TestFramework.h"
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

class FileEncryptionTest {
public:
    static bool testEncryptDecryptFile() {
        // Setup test files
        const std::string testFile = "test_encrypt.txt";
        const std::string encryptedFile = "test_encrypt.enc";
        const std::string decryptedFile = "test_decrypt.txt";
        const std::string password = "testPassword123";
        const std::string testContent = "This is a test file for encryption and decryption.";
        
        // Clean up any existing test files
        if (std::filesystem::exists(testFile)) {
            std::filesystem::remove(testFile);
        }
        if (std::filesystem::exists(encryptedFile)) {
            std::filesystem::remove(encryptedFile);
        }
        if (std::filesystem::exists(decryptedFile)) {
            std::filesystem::remove(decryptedFile);
        }
        
        // Create a test file
        std::ofstream file(testFile);
        file << testContent;
        file.close();
        
        // Encrypt the file
        FileEncryption fileEncryptor;
        bool encryptResult = fileEncryptor.encryptFile(testFile, encryptedFile, password);
        ASSERT_TRUE(encryptResult);
        ASSERT_TRUE(std::filesystem::exists(encryptedFile));
        
        // Decrypt the file
        bool decryptResult = fileEncryptor.decryptFile(encryptedFile, decryptedFile, password);
        ASSERT_TRUE(decryptResult);
        ASSERT_TRUE(std::filesystem::exists(decryptedFile));
        
        // Verify the content of the decrypted file
        std::ifstream decryptedFileStream(decryptedFile);
        std::string decryptedContent((std::istreambuf_iterator<char>(decryptedFileStream)),
                                    std::istreambuf_iterator<char>());
        decryptedFileStream.close();
        
        ASSERT_EQUAL(testContent, decryptedContent);
        
        // Clean up
        std::filesystem::remove(testFile);
        std::filesystem::remove(encryptedFile);
        std::filesystem::remove(decryptedFile);
        
        return true;
    }
    
    static bool testEncryptDecryptWithWrongPassword() {
        // Setup test files
        const std::string testFile = "test_encrypt.txt";
        const std::string encryptedFile = "test_encrypt.enc";
        const std::string decryptedFile = "test_decrypt.txt";
        const std::string correctPassword = "correctPassword";
        const std::string wrongPassword = "wrongPassword";
        const std::string testContent = "This is a test file for encryption and decryption.";
        
        // Clean up any existing test files
        if (std::filesystem::exists(testFile)) {
            std::filesystem::remove(testFile);
        }
        if (std::filesystem::exists(encryptedFile)) {
            std::filesystem::remove(encryptedFile);
        }
        if (std::filesystem::exists(decryptedFile)) {
            std::filesystem::remove(decryptedFile);
        }
        
        // Create a test file
        std::ofstream file(testFile);
        file << testContent;
        file.close();
        
        // Encrypt the file
        FileEncryption fileEncryptor;
        bool encryptResult = fileEncryptor.encryptFile(testFile, encryptedFile, correctPassword);
        ASSERT_TRUE(encryptResult);
        
        // Try to decrypt with wrong password
        bool decryptResult = fileEncryptor.decryptFile(encryptedFile, decryptedFile, wrongPassword);
        ASSERT_FALSE(decryptResult); // Should fail with wrong password
        
        // Clean up
        std::filesystem::remove(testFile);
        std::filesystem::remove(encryptedFile);
        if (std::filesystem::exists(decryptedFile)) {
            std::filesystem::remove(decryptedFile);
        }
        
        return true;
    }

};