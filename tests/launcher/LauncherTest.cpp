#include <windows.h>
#include <string>
#include <iostream>
#include <filesystem>
#include "TestFramework.h"

class LauncherTest {
public:
    static bool testSetConsoleColor() {
        try {
            WORD colors[] = {
                FOREGROUND_RED,
                FOREGROUND_GREEN,
                FOREGROUND_BLUE,
                FOREGROUND_RED | FOREGROUND_GREEN,
                FOREGROUND_RED | FOREGROUND_BLUE,
                FOREGROUND_GREEN | FOREGROUND_BLUE,
                FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
                FOREGROUND_INTENSITY
            };

            for (WORD color : colors) {
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, color);
            }

            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

            return true;
        } catch (...) {
            return false;
        }
    }

    static bool testGetAvailableDrive() {
        std::string drive = getAvailableDrive();

        ASSERT_TRUE(!drive.empty());
        ASSERT_TRUE(drive.length() >= 2);
        ASSERT_TRUE(drive[1] == ':');
        ASSERT_TRUE(drive[2] == '\\');

        ASSERT_TRUE(std::filesystem::exists(drive));

        return true;
    }


    static bool testProcessCreationFailure() {
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        bool success = CreateProcessA(
            "nonexistent_executable.exe",
            NULL,
            NULL,
            NULL,
            FALSE,
            CREATE_NO_WINDOW,
            NULL,
            NULL,
            &si,
            &pi
        );

        ASSERT_TRUE(!success); //should fail

        return true;
    }

    static std::string getAvailableDrive() {
        const std::string drives[] = {"D:\\", "F:\\", "E:\\", "G:\\"};
        for (const auto& drive : drives) {
            if (std::filesystem::exists(drive)) {
                return drive;
            }
        }
        return "C:\\";
    }
};
