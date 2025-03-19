#include "TestFramework.h"
#include "terminal/CommandImplementationTest.cpp"
#include "terminal/CompileAndRunTest.cpp"
#include "terminal/CommandParserTest.cpp"
#include "terminal/TerminalTest.cpp"
#include "LauncherTest.cpp"
#include "encryption/FileEncryptionTest.cpp"

int main(){
    TestSuite masterSuite;

    masterSuite.addTest("File Creation Test", CommandImplementationTest::testCreateFileCommand);
    masterSuite.addTest("Directory Creation Test", CommandImplementationTest::testCreateDirectoryCommand);
    masterSuite.addTest("Copy File Test", CommandImplementationTest::testCopyCommand);
    masterSuite.addTest("Remove File Test", CommandImplementationTest::testRemoveCommand);
    masterSuite.addTest("Move File Test", CommandImplementationTest::testMoveCommand);
    masterSuite.addTest("Rename File Test", CommandImplementationTest::testRenameCommand);
    masterSuite.addTest("Current Directory Test", CommandImplementationTest::testCurrentDirectoryCommand);
    masterSuite.addTest("List Directory Test", CommandImplementationTest::testListDirectoryCommand);
    masterSuite.addTest("Tree Directory Test", CommandImplementationTest::testTreeCommand);
    masterSuite.addTest("Find File Test", CommandImplementationTest::testFindCommand);

    masterSuite.addTest("Run C++ File Test", CompileAndRunTest::testRunCppCommand);
    masterSuite.addTest("Run Python File Test", CompileAndRunTest::testRunPythonCommand);
    masterSuite.addTest("Run Java File Test", CompileAndRunTest::testRunJavaCommand);
    masterSuite.addTest("Run Invalid File Test", CompileAndRunTest::testRunWithInvalidFile);
    masterSuite.addTest("Run Compilation Error Test", CompileAndRunTest::testRunWithCompilationError);
    masterSuite.addTest("Run Unsupported Extension Test", CompileAndRunTest::testRunWithUnsupportedExtension);

    masterSuite.addTest("Register Command Test", CommandParserTest::testRegisterCommand);
    masterSuite.addTest("Execute Command Test", CommandParserTest::testExecuteCommand);
    masterSuite.addTest("Execute Command With Exception Test", CommandParserTest::testExecuteCommandWithException);
    masterSuite.addTest("Is Valid Command Test", CommandParserTest::testIsValidCommand);
    masterSuite.addTest("Get Command List Test", CommandParserTest::testGetCommandList);
    masterSuite.addTest("Initialize Default Commands Test", CommandParserTest::testInitializeDefaultCommands);

    masterSuite.addTest("Terminal Running State Test", TerminalTest::testTerminalRunningState);

    masterSuite.addTest("Set Console Color Test", LauncherTest::testSetConsoleColor);
    masterSuite.addTest("Get Available Drive Test", LauncherTest::testGetAvailableDrive);
    masterSuite.addTest("Process Creation Failure Test", LauncherTest::testProcessCreationFailure);

    // File Encryption Tests
    masterSuite.addTest("Encrypt Decrypt File Test", FileEncryptionTest::testEncryptDecryptFile);
    masterSuite.addTest("Encrypt Decrypt With Wrong Password Test", FileEncryptionTest::testEncryptDecryptWithWrongPassword);
    masterSuite.runAll();

    return 0;
}
