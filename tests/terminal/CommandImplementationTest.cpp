#include "terminal/CommandImplementation.h"
#include "terminal/Terminal.h"
#include "../TestFramework.h"
#include <filesystem>
#include <vector>
#include <string>

class CommandImplementationTest{
    public:
        static bool testCreateFileCommand(){

            Terminal terminal;

            const std::string testFile = "test.txt";

            if(std::filesystem::exists(testFile)){
                std::filesystem::remove(testFile);
            }

            std::vector<std::string> args;
            args.push_back(testFile);

            terminal.executeCommand("fcreate", args);
            ASSERT_TRUE(std::filesystem::exists(testFile));
            std::filesystem::remove(testFile);

            return true;

        }
};