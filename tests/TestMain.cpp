#include <iostream>
#include "TestFramework.h"
#include "terminal/CommandImplementationTest.cpp"
#include "terminal/CompileAndRunTest.cpp"

int main(){
    TestSuite masterSuite;

    // masterSuite.addTest("File Creation Test", CommandImplementationTest::testCreateFileCommand);
    // masterSuite.addTest("Directory Creation Test", CommandImplementationTest::testCreateDirectoryCommand);
    // masterSuite.addTest("Copy File Test", CommandImplementationTest::testCopyCommand);
    // masterSuite.addTest("Remove File Test", CommandImplementationTest::testRemoveCommand);
    // masterSuite.addTest("Move File Test", CommandImplementationTest::testMoveCommand);
    // masterSuite.addTest("Rename File Test", CommandImplementationTest::testRenameCommand);
    // masterSuite.addTest("Current Directory Test", CommandImplementationTest::testCurrentDirectoryCommand);
    // masterSuite.addTest("List Directory Test", CommandImplementationTest::testListDirectoryCommand);
    // masterSuite.addTest("Tree Directory Test", CommandImplementationTest::testTreeCommand);
    // masterSuite.addTest("Find File Test", CommandImplementationTest::testFindCommand);

    masterSuite.addTest("Run C++ File Test", CompileAndRunTest::testRunCppCommand);
    masterSuite.addTest("Run Python File Test", CompileAndRunTest::testRunPythonCommand);
    masterSuite.addTest("Run Java File Test", CompileAndRunTest::testRunJavaCommand);
    masterSuite.addTest("Run Invalid File Test", CompileAndRunTest::testRunWithInvalidFile);
    masterSuite.addTest("Run Compilation Error Test", CompileAndRunTest::testRunWithCompilationError);
    masterSuite.addTest("Run Unsupported Extension Test", CompileAndRunTest::testRunWithUnsupportedExtension);

    masterSuite.runAll();

    return 0;
}
