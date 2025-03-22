# SecureShell
Welcome to SecureShell, a command-line application developed in C++ using modern C++ libraries. This application enables users to efficiently navigate the Windows file system, execute cmd-let operations, and file encryption and decryption. It also supports multi-language program compilation with a single command and users can secure sensitive data with built-in password management functionalities.

# Key Features

### File and Directory Management:
- Create, copy, move, rename, and delete files and directories.

- List directory contents and display file permissions.

- Navigate through directories using the cd command.



 ```bash
  ls /path/to/directory
  cd /path/to/directory     # Change Directory
  dcreate new_directory     # Create a Directory
  copy source.txt destination.txt    # Copy Files 
  move source.txt /new/location/      # Move Files
  remove file.txt
  remove directory/

```


 
 ### Compile and Run Code:

- Compile and run code files directly from the terminal with a single command called 'run'.
- Languages: C, C++, Python, Java, JavaScript and Rust

 ```bash
  run file.py
  run file.js
...
```

  
 ### Encryption and Decryption:

- Secure file encryption and decryption using custom algorithms.

- Password-based encryption for sensitive files.


- ##### Encrypt a file:

```bash
 encrypt input.txt output.enc password
```

- ##### Decrypt a file:
```bash
decrypt output.enc decrypted.txt password
```  




### Password Manager:

- Secure storage and retrieval of passwords.

- Master password protection with strong password validation.

- Password generation and management for various services.

##### - Use the passman command to access the password manager.


```bash
passman
```

![img](https://github.com/user-attachments/assets/ec011b5c-c95a-47bb-ad37-974ac0ce56b0)


### Additional Commands:

- Support for custom commands like write, cat, head, tree, and find.

```bash
write file.txt         #write in a file
write file.txt -a      #apprend to a file
cat file.txt           #view file contents
```




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

#Usage

## Main Features




### 4.System Information:

Retrieve system information using the sysinfo command:

```bash
sysinfo
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


# Supervised by

### Ishmam Tashdeed 

Lecturer
Department of Computer Science and Engineering
Islamic University of Technology



