cmake_minimum_required(VERSION 3.15)
project(SecureShell VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files for main terminal application
add_executable(SecureShell
    src/main.cpp
    src/terminal/Terminal.cpp
    src/terminal/CommandParser.cpp
    src/passman/PasswordManager.cpp
    src/passman/PasswordStorage.cpp
    src/passman/PasswordCrypto.cpp
    src/encryption/FileEncryption.cpp
    src/config/ConfigManager.cpp
    src/utils/Utils.cpp
)

# Add launcher application that creates a new window
add_executable(SecureShellLauncher
    src/launcher.cpp
)

# Include directories
target_include_directories(SecureShell PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Set output directory for both executables
set_target_properties(SecureShell SecureShellLauncher PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy configuration file to build directory
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/terminal_config.txt
               ${CMAKE_BINARY_DIR}/bin/terminal_config.txt
               COPYONLY)