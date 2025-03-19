#include "terminal/CommandImplementation.h"
#include "terminal/Terminal.h"
#include "../TestFramework.h"
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

class CompileAndRunTest {
public:
    static bool testRunCppCommand() {
        Terminal terminal;
        const std::string testFile = "test_run.cpp";

        std::ofstream file(testFile);
        file << "#include <iostream>\n";
        file << "int main() {\n";
        file << "    std::cout << \"Hello from C++\" << std::endl;\n";
        file << "    return 0;\n";
        file << "}\n";
        file.close();

        std::vector<std::string> args;
        args.push_back(testFile);

        terminal.executeCommand("run", args);

        std::filesystem::remove(testFile);
        if (std::filesystem::exists("test_run.exe")) {
            std::filesystem::remove("test_run.exe");
        }

        return true;
    }

    static bool testRunPythonCommand() {
        Terminal terminal;
        const std::string testFile = "test_run.py";

        std::ofstream file(testFile);
        file << "print('Hello from Python')\n";
        file.close();

        std::vector<std::string> args;
        args.push_back(testFile);

        terminal.executeCommand("run", args);

        std::filesystem::remove(testFile);

        return true;
    }

    static bool testRunJavaCommand() {
        Terminal terminal;
        const std::string testFile = "TestRun.java";

        // Create a simple Java test file
        std::ofstream file(testFile);
        file << "public class TestRun {\n";
        file << "    public static void main(String[] args) {\n";
        file << "        System.out.println(\"Hello from Java\");\n";
        file << "    }\n";
        file << "}\n";
        file.close();

        std::vector<std::string> args;
        args.push_back(testFile);

        terminal.executeCommand("run", args);

        // Clean up
        std::filesystem::remove(testFile);
        if (std::filesystem::exists("TestRun.class")) {
            std::filesystem::remove("TestRun.class");
        }

        return true;
    }

    static bool testRunWithInvalidFile() {
        Terminal terminal;
        const std::string testFile = "nonexistent.cpp";

        std::vector<std::string> args;
        args.push_back(testFile);

        terminal.executeCommand("run", args);

        // The command should handle the error gracefully
        ASSERT_TRUE(!std::filesystem::exists("nonexistent.exe"));

        return true;
    }

    static bool testRunWithCompilationError() {
        Terminal terminal;
        const std::string testFile = "error_test.cpp";

        // Create a C++ file with syntax error
        std::ofstream file(testFile);
        file << "#include <iostream>\n";
        file << "int main() {\n";
        file << "    std::cout << \"This has an error << std::endl;\n"; // Missing closing quote
        file << "    return 0;\n";
        file << "}\n";
        file.close();

        std::vector<std::string> args;
        args.push_back(testFile);

        terminal.executeCommand("run", args);

        // Clean up
        std::filesystem::remove(testFile);
        ASSERT_TRUE(!std::filesystem::exists("error_test.exe"));

        return true;
    }

    static bool testRunWithUnsupportedExtension() {
        Terminal terminal;
        const std::string testFile = "test.unsupported";

        // Create a file with unsupported extension
        std::ofstream file(testFile);
        file << "This is a test file with unsupported extension";
        file.close();

        std::vector<std::string> args;
        args.push_back(testFile);

        terminal.executeCommand("run", args);

        // Clean up
        std::filesystem::remove(testFile);

        return true;
    }
};
