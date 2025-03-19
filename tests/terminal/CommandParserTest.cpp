#include "terminal/CommandParser.h"
#include "../TestFramework.h"
#include <vector>
#include <string>
#include <functional>

class CommandParserTest {
public:
    static bool testRegisterCommand() {
        CommandParser parser;
        bool commandExecuted = false;

        parser.registerCommand("test", [&commandExecuted](const std::vector<std::string>& args) {
            commandExecuted = true;
        });

        ASSERT_TRUE(parser.isValidCommand("test"));

        std::vector<std::string> args;
        bool result = parser.executeCommand("test", args);
        ASSERT_TRUE(result);
        ASSERT_TRUE(commandExecuted);

        return true;
    }

    static bool testExecuteCommand() {
        CommandParser parser;
        std::string capturedArg;

        parser.registerCommand("echo", [&capturedArg](const std::vector<std::string>& args) {
            if (!args.empty()) {
                capturedArg = args[0];
            }
        });

        std::vector<std::string> args = {"hello"};
        bool result = parser.executeCommand("echo", args);
        ASSERT_TRUE(result);
        ASSERT_EQUAL("hello", capturedArg);

        result = parser.executeCommand("nonexistent", args);
        ASSERT_TRUE(!result);

        return true;
    }

    static bool testExecuteCommandWithException() {
        CommandParser parser;

        parser.registerCommand("fail", [](const std::vector<std::string>& args) {
            throw std::runtime_error("Test exception");
        });

        std::vector<std::string> args;
        bool result = parser.executeCommand("fail", args);

        ASSERT_TRUE(!result);

        return true;
    }


    static bool testIsValidCommand() {
        CommandParser parser;

        ASSERT_TRUE(parser.isValidCommand("help"));
        ASSERT_TRUE(parser.isValidCommand("exit"));
        ASSERT_TRUE(parser.isValidCommand("ls"));

        ASSERT_TRUE(!parser.isValidCommand("nonexistent"));

        parser.registerCommand("newcommand", [](const std::vector<std::string>&) {});
        ASSERT_TRUE(parser.isValidCommand("newcommand"));

        return true;
    }

    static bool testInitializeDefaultCommands() {
        CommandParser parser;

        ASSERT_TRUE(parser.isValidCommand("help"));
        ASSERT_TRUE(parser.isValidCommand("exit"));
        ASSERT_TRUE(parser.isValidCommand("cd"));
        ASSERT_TRUE(parser.isValidCommand("ls"));
        ASSERT_TRUE(parser.isValidCommand("run"));

        auto commandList = parser.getCommandList();

        bool hasHelp = false;
        bool hasExit = false;
        bool hasLs = false;

        for (const auto& [cmd, desc] : commandList) {
            if (cmd == "help") {
                hasHelp = true;
                ASSERT_EQUAL("Display this help message", desc);
            }
            if (cmd == "exit") {
                hasExit = true;
                ASSERT_EQUAL("Exit the terminal", desc);
            }
            if (cmd == "ls") {
                hasLs = true;
                ASSERT_EQUAL("List directory contents", desc);
            }
        }

        ASSERT_TRUE(hasHelp);
        ASSERT_TRUE(hasExit);
        ASSERT_TRUE(hasLs);

        return true;
    }
};
