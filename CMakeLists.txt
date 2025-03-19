cmake_minimum_required(VERSION 3.15)
project(SecureShell VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create component libraries
add_library(terminal_lib
    src/terminal/Terminal.cpp
    src/terminal/CommandParser.cpp
    src/terminal/CommandImplementation.cpp
    src/terminal/FileOperations.cpp
)

add_library(encryption_lib
    src/encryption/FileEncryption.cpp
    src/encryption/EncryptionHandler.cpp
)

add_library(passman_lib
    src/passman/PasswordManager.cpp
    src/passman/PasswordStorage.cpp
    src/passman/PasswordCrypto.cpp
    src/passman/PasswordManagerOperations.cpp
)

add_library(utils_lib
    src/utils/Utils.cpp
    src/utils/UtilsHelpers.cpp
)

add_library(config_lib
    src/config/ConfigManager.cpp
)

# Link libraries dependencies
target_link_libraries(terminal_lib
    PRIVATE
    encryption_lib
    passman_lib
    utils_lib
    config_lib
)

# Include directories for libraries
target_include_directories(terminal_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(encryption_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(passman_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(utils_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(config_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add main executable
add_executable(SecureShell src/main.cpp)

# Link main executable
target_link_libraries(SecureShell 
    PRIVATE 
    terminal_lib
    encryption_lib
    passman_lib
    utils_lib
    config_lib
)

# Add launcher application
add_executable(SecureShellLauncher src/launcher.cpp)

# Add test executable
add_executable(command_tests tests/TestMain.cpp
        tests/terminal/CompileAndRunTest.cpp
        tests/terminal/CommandParserTest.cpp
        tests/terminal/TerminalTest.cpp
        tests/launcher/LauncherTest.cpp)

# Link test executable
target_link_libraries(command_tests
    PRIVATE
    terminal_lib
    encryption_lib
    passman_lib
    utils_lib
    config_lib
)

# Include directories for test
target_include_directories(command_tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# Set output directory for executables
set_target_properties(SecureShell SecureShellLauncher command_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy configuration file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/terminal_config.txt
               ${CMAKE_BINARY_DIR}/bin/terminal_config.txt
               COPYONLY)