#include <iostream>
#include "TestFramework.h"
#include "terminal/CommandImplementationTest.cpp"

int main(){
    TestSuite masterSuite;

    masterSuite.addTest("File Creation Test", CommandImplementationTest::testCreateFileCommand);

    masterSuite.runAll();

    return 0;
}