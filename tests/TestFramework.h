#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <filesystem>

class TestSuite{

    struct Test{
        std::string name;
        std::function<bool()> testFunc;
    };

    std::vector<Test> tests;

    public:

        void addTest(const std::string name, std::function<bool()> testFunc){
            tests.push_back({name, testFunc});
        }

        void runAll(){
            std::cout<<"\n Running all tests... \n";

            int passed = 0;
            int failed = 0;

            for(const auto& test : tests){

                std::cout<<"Running test : "<<test.name<< std::endl;

                bool result = test.testFunc();

                if(result){
                    std::cout<<"PASSED\n"<<std::endl;
                    passed++;
                }
                else{
                    std::cout<<"FAILED\n"<<std::endl;
                    failed++;
                }
            }
            
            std::cout<<"Test Summary : "<<std::endl;
            std::cout<<passed<<"/"<<tests.size()<<" tests passed"<<std::endl;
            std::cout<<failed<<" tests failed\n"<<std::endl;
        }
};

#define ASSERT_TRUE(condition) \
    if(!(condition)){ \
        std::cout<<"Assertion failed at "<<__FILE__<<":"<<__LINE__<<std::endl; \
        std::cout<<"Expected false: "<<#condition<<std::endl; \
        return false; \
    }

#define ASSERT_FALSE(condition) \
    if(condition){ \
        std::cout<<"Assertion failed at "<<__FILE__<<":"<<__LINE__<<std::endl; \
        std::cout<<"Expected false: "<<#condition<<std::endl; \
        return false; \
    }

#define ASSERT_EQUAL(expected, actual) \
    if(expected != actual){ \
    std::cout<<"Assertion failed at "<<__FILE__<<":"<<__LINE__<<std::endl; \
        std::cout<<"Expected : "<<expected<<std::endl; \
        std::cout<<"Actual : "<<actual<<std::endl; \
        return false; \
    }