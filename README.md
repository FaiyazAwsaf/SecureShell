# SecureShell
Welcome to SecureShell, a command-line application developed in C++ using modern C++ libraries. This application enables users to efficiently navigate the Windows file system, execute cmd-let operations, and manage files with enhanced security features. SecureShell also supports multi-language program compilation with a single command, providing a seamless development experience. Additionally, users can encrypt and decrypt files using various encryption techniques and secure sensitive data with built-in password management functionalities.

# Motivation Behind the Project

SecureShell Terminal was born out of our interest for cybersecurity and the desire to create a tool that bridges the gap between traditional command-line interfaces and modern security practices. We aimed to explore and implement secure file handling, encryption, password management, and compilation of code files from scratch.

# Key Features

### File and Directory Management:
- Create, copy, move, rename, and delete files and directories.

- List directory contents and display file permissions.

- Navigate through directories using the cd command.
  
 ### Encryption and Decryption:

- Secure file encryption and decryption using custom algorithms.

- Password-based encryption for sensitive files.

### Password Manager:

- Secure storage and retrieval of passwords.

- Master password protection with strong password validation.

- Password generation and management for various services.

### System Information:

- Commands to retrieve system information (CPU, memory, OS details).

### Custom Commands:

- Support for custom commands like grep, cat, head, tree, and find.

- Compile and run code files directly from the terminal.

### Cross-Platform Support:

- Designed to work on both Windows and Unix-based systems.

# Installation
To run SecureShell Terminal on your local machine, follow these steps:

### 1. Prerequisites:
- Ensure you have a C++ compiler installed (e.g., GCC, Clang, or MSVC).
- Install [CMake](https://cmake.org/) (version 3.10 or higher).

### 2. Clone the Repository:
```bash
git clone https://github.com/your-username/SecureShell-Terminal.git
cd SecureShell-Terminal

```

### 3. Build the Project

```bash
mkdir build
cd build
cmake ..
make
```

### 4. Run the Application:

On Windows

```bash
SecureShell.exe
```

On Unix-based systems:

```bash
./SecureShell
```

#Usage

## Main Features

### 1.File Management:

- ##### List Directory Contents:
Use the ls command to list files and directories in the current folder.

 ```bash
  ls /path/to/directory
  ```
- ##### Change Directory:
Use the cd command to navigate to a different directory.
```bash
cd /path/to/directory
```

- ##### Create a Directory:
Use the dcreate command to create a new directory.
```bash
dcreate new_directory
```
- ##### Copy Files:
Use the copy command to copy a file from one location to another.
```bash
copy source.txt destination.txt
```

- ##### Move Files:
Use the move command to move a file to a new location.

 ```bash
move source.txt /new/location/
 ```
- ##### Remove Files or Directories:
Use the remove command to delete a file or directory.

 ```bash
remove file.txt
remove directory/
```


### 2.Encryption and Decryption:

- Encrypt a file:

```bash
 encrypt input.txt output.enc mypassword
```

- Decrypt a file:
```bash
decrypt output.enc decrypted.txt mypassword
```  

### 3.Password Manager :
##### - Access the Password Manager:
Use the passman command to access the password manager.


```bash
passman
```

![img](https://github.com/user-attachments/assets/ec011b5c-c95a-47bb-ad37-974ac0ce56b0)


### 4.System Information:

Retrieve system information using the sysinfo command:

```bash
sysinfo
```

### 5.Custom Commands:

- Use commands like grep, cat, head, tree, and find for advanced file operations.
- Example :
  
```bash
grep "pattern" file.txt
cat file.txt
```

# Tools and Technologies
- Programming Language: C++

- Libraries:

  -C++ Standard Library: For file handling, string manipulation, and data structures.

  -Filesystem Library: For cross-platform file and directory operations.

  -Windows API: For console manipulation and process creation on Windows.

- Encryption:

  -Custom encryption algorithms (XOR and Caesar cipher).

  -Secure hashing and salting for password management.

- Build Tools:

  -CMake: For building the project across different platforms.

  -GCC/Clang: Compilers used for building the C++ code.

  -Version Control: Git
  
# Developers
This project was developed by:

- [Faiyaz Ahmad Awsaf](https://github.com/FaiyazAwsaf)
- [Mashrur Faiyaz](https://github.com/lemurz)
- [Md. Sadman Mubasshir Khan](https://github.com/mksadman)
- [Nanziba Razin Samita](https://github.com/NanzibaSamita)

This project was developed by us for the Software Project Lab 1 (SWE 4301) during the 3rd semester at the Islamic University of Technology(IUT).

# Supervised by

### Ishmam Tashdeed 

Lecturer

Department of Computer Science and Engineering

Islamic University of Technology


