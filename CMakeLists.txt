cmake_minimum_required(VERSION 3.15)
project(SecureShell VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
add_executable(SecureShell
    src/main.cpp
    src/terminal/Terminal.cpp
    src/terminal/CommandParser.cpp
    src/passman/PasswordManager.cpp
    src/encryption/FileEncryption.cpp
    src/config/ConfigManager.cpp
    src/utils/Utils.cpp
        src/main.cpp
)

# Include directories
target_include_directories(SecureShell PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Set output directory
set_target_properties(SecureShell PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy configuration file to build directory
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/terminal_config.txt
               ${CMAKE_BINARY_DIR}/bin/terminal_config.txt
               COPYONLY)