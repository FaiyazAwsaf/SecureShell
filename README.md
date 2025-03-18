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

- Use commands like ls, cd, copy, move, and remove to manage files and directories.
- Example:

```bash
ls /path/to/directory
copy source.txt destination.txt
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

- Access the password manager using the passman command.
- Add, retrieve, update, and remove passwords for various services.

```bash
passman
```
Follow the on-screen instructions to manage your passwords.

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



