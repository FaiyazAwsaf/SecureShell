#include <windows.h>
#include <string>
#include <iostream>
#include <filesystem>

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