#include "terminal/CommandImplementation.h"
#include "terminal/Terminal.h"
#include "../TestFramework.h"
#include <filesystem>
#include <fstream>
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

        static bool testCreateDirectoryCommand() {
            Terminal terminal;
            const std::string testDir = "test_dir";
    
            if(std::filesystem::exists(testDir)) {
                std::filesystem::remove_all(testDir);
            }
    
            std::vector<std::string> args;
            args.push_back(testDir);
    
            terminal.executeCommand("dcreate", args);
            ASSERT_TRUE(std::filesystem::exists(testDir));
            std::filesystem::remove_all(testDir);
    
            return true;
        }

        static bool testCopyCommand() {
            Terminal terminal;
            const std::string sourceFile = "source.txt";
            const std::string destFile = "dest.txt";

            std::ofstream source(sourceFile);
            source << "test content";
            source.close();
    
            std::vector<std::string> args;
            args.push_back(sourceFile);
            args.push_back(destFile);
    
            terminal.executeCommand("copy", args);
            ASSERT_TRUE(std::filesystem::exists(destFile));
    
            std::filesystem::remove(sourceFile);
            std::filesystem::remove(destFile);
    
            return true;
        }

        static bool testRemoveCommand() {
            Terminal terminal;
            const std::string testFile = "to_remove.txt";

            std::ofstream file(testFile);
            file.close();
    
            std::vector<std::string> args;
            args.push_back(testFile);
    
            terminal.executeCommand("remove", args);
            ASSERT_TRUE(!std::filesystem::exists(testFile));
    
            return true;
        }

        static bool testMoveCommand() {
            Terminal terminal;
            const std::string sourceFile = "source_move.txt";
            const std::string destFile = "dest_move.txt";
    
            std::ofstream source(sourceFile);
            source << "test content";
            source.close();
    
            std::vector<std::string> args;
            args.push_back(sourceFile);
            args.push_back(destFile);
    
            terminal.executeCommand("move", args);
            ASSERT_TRUE(std::filesystem::exists(destFile));
            ASSERT_TRUE(!std::filesystem::exists(sourceFile));
    
            std::filesystem::remove(destFile);
            return true;
        }

        static bool testRenameCommand() {
            Terminal terminal;
            const std::string oldName = "old_name.txt";
            const std::string newName = "new_name.txt";
    
            std::ofstream file(oldName);
            file.close();
    
            std::vector<std::string> args;
            args.push_back(oldName);
            args.push_back(newName);
    
            terminal.executeCommand("rename", args);
            ASSERT_TRUE(std::filesystem::exists(newName));
            ASSERT_TRUE(!std::filesystem::exists(oldName));
    
            std::filesystem::remove(newName);
            return true;
        }

        static bool testCurrentDirectoryCommand() {
            Terminal terminal;
            std::vector<std::string> args;
            
            auto currentPath = std::filesystem::current_path();
            terminal.executeCommand("curr", args);
            ASSERT_TRUE(std::filesystem::current_path() == currentPath);
            
            return true;
        }

        static bool testListDirectoryCommand() {
            Terminal terminal;
            const std::string testDir = "ls_test_dir";
            const std::string testFile = "ls_test_dir/test.txt";
    
            std::filesystem::create_directory(testDir);
            std::ofstream file(testFile);
            file.close();
    
            std::vector<std::string> args;
            args.push_back(testDir);
    
            terminal.executeCommand("ls", args);
    
            std::filesystem::remove_all(testDir);
            return true;
        }

        static bool testTreeCommand() {
            Terminal terminal;
            const std::string testDir = "tree_test_dir";
            const std::string subDir = "tree_test_dir/subdir";
            const std::string testFile = "tree_test_dir/test.txt";
    
            std::filesystem::create_directory(testDir);
            std::filesystem::create_directory(subDir);
            std::ofstream file(testFile);
            file.close();
    
            std::vector<std::string> args;
            args.push_back(testDir);
    
            terminal.executeCommand("tree", args);
    
            std::filesystem::remove_all(testDir);
            return true;
        }

        static bool testFindCommand() {
            Terminal terminal;
            const std::string testDir = "find_test_dir";
            const std::string testFile = "find_test_dir/findme.txt";
    
            std::filesystem::create_directory(testDir);
            std::ofstream file(testFile);
            file.close();
    
            std::vector<std::string> args;
            args.push_back("findme");
    
            terminal.executeCommand("find", args);
            
            std::filesystem::remove_all(testDir);
            return true;
        }


};