#include <windows.h>
#include <string>
#include <iostream>
#include <filesystem>

// Function to set console text color
void setConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

int main() {
    // Get the current executable path
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    // Get the directory of the current executable
    std::filesystem::path exeDirectory = std::filesystem::path(exePath).parent_path();

    // Construct the path to the main terminal executable
    // Assuming the main terminal executable will be named SecureShell.exe
    std::filesystem::path terminalPath = exeDirectory / "SecureShell.exe";

    // Create process information structures
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    // Initialize the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Convert path to string
    std::string terminalPathStr = terminalPath.string();

    // Set the welcome message color to green
    setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "-------------Welcome to SecureShell Terminal!-------------\n\n";
    std::cout << "Type 'help' for a list of available commands.\n\n";

    // Reset the text color to default (gray)
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    // Create a new console window and start the terminal process
    if (!CreateProcessA(
        terminalPathStr.c_str(),  // Application name
        NULL,                      // Command line arguments
        NULL,                      // Process security attributes
        NULL,                      // Thread security attributes
        FALSE,                     // Handle inheritance
        CREATE_NEW_CONSOLE,        // Creation flags - create a new console window
        NULL,                      // Environment
        "D:\\",                    // Current directory - set to system root
        &si,                       // Startup info
        &pi                        // Process information
    )) {
        std::cerr << "Failed to create process. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}