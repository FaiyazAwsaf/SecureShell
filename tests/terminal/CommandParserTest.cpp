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

    static bool testParseInput() {
        CommandParser parser;

        std::string input = "command arg1 arg2";
        std::vector<std::string> tokens = parser.parseInput(input);
        ASSERT_EQUAL(3, tokens.size());
        ASSERT_EQUAL("command", tokens[0]);
        ASSERT_EQUAL("arg1", tokens[1]);
        ASSERT_EQUAL("arg2", tokens[2]);

        input = "command \"quoted arg\" arg2";
        tokens = parser.parseInput(input);
        ASSERT_EQUAL(3, tokens.size());
        ASSERT_EQUAL("command", tokens[0]);
        ASSERT_EQUAL("\"quoted arg\"", tokens[1]);
        ASSERT_EQUAL("arg2", tokens[2]);

        input = "";
        tokens = parser.parseInput(input);
        ASSERT_EQUAL(0, tokens.size());

        input = "   ";
        tokens = parser.parseInput(input);
        ASSERT_EQUAL(0, tokens.size());

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

    static bool testGetCommandList() {
        CommandParser parser;

        auto commandList = parser.getCommandList();

        ASSERT_TRUE(!commandList.empty());

        bool hasHelp = false;
        bool hasExit = false;

        for (const auto& [cmd, desc] : commandList) {
            if (cmd == "help") {
                hasHelp = true;
                ASSERT_EQUAL("Display this help message", desc);
            }
            if (cmd == "exit") {
                hasExit = true;
                ASSERT_EQUAL("Exit the terminal", desc);
            }
        }

        ASSERT_TRUE(hasHelp);
        ASSERT_TRUE(hasExit);

        parser.registerCommand("newcmd", [](const std::vector<std::string>&) {});
        commandList = parser.getCommandList();

        bool hasNewCmd = false;
        for (const auto& [cmd, desc] : commandList) {
            if (cmd == "newcmd") {
                hasNewCmd = true;
                break;
            }
        }

        ASSERT_TRUE(hasNewCmd);

        return true;
    }

    static bool testSplitString() {
        CommandParser parser;

        std::string input = "one,two,three";
        std::vector<std::string> parts = parser.splitString(input, ',');
        ASSERT_EQUAL(3, parts.size());
        ASSERT_EQUAL("one", parts[0]);
        ASSERT_EQUAL("two", parts[1]);
        ASSERT_EQUAL("three", parts[2]);

        input = "one,,three";
        parts = parser.splitString(input, ',');
        ASSERT_EQUAL(3, parts.size());
        ASSERT_EQUAL("one", parts[0]);
        ASSERT_EQUAL("", parts[1]);
        ASSERT_EQUAL("three", parts[2]);

        input = "";
        parts = parser.splitString(input, ',');
        ASSERT_EQUAL(1, parts.size());
        ASSERT_EQUAL("", parts[0]);

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
