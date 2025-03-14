#include <windows.h>
#include <string>
#include <iostream>
#include <filesystem>

// Function to set console text color
void setConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

std::string getAvailableDrive() {
    const std::string drives[] = {"D:\\", "F:\\", "E:\\", "G:\\"};
    for (const auto& drive : drives) {
        if (std::filesystem::exists(drive)) {
            return drive;
        }
    }
    return "C:\\"; // Default to C:\ if none are available
}

int main() {

    // Create process information structures
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    // Initialize the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));


    // Set the welcome message color to green
    setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "-------------   Welcome to SecureShell Terminal   -------------\n\n";
    std::cout << "Type 'help' for a list of available commands.\n\n";

    // Reset the text color to default (gray)
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    std::string workingDirectory = getAvailableDrive();

    if (!CreateProcessA(
        "SecureShell.exe",         // Application name
        NULL,
        NULL,
        NULL,
        FALSE,
        CREATE_NEW_CONSOLE,
        NULL,
        workingDirectory.c_str(),  // Current directory
        &si,                       // Startup info
        &pi                        // Process info
    )) {
        std::cerr << "Failed to create process. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}