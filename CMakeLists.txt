cmake_minimum_required(VERSION 3.29)
project(SecureShell)

set(CMAKE_CXX_STANDARD 14)

include_directories(encryptionLib)

add_executable(SecureShell
    encryptionLib/SimpleEncryptLib.cpp
    encryptionLib/SimpleEncryptLib.h
        PasswordManager.cpp)
