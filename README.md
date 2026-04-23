# ShebangWin

**ShebangWin** is a lightweight Windows utility that brings "shebang-like" behavior to any programming language. It reads the first line of a source file, extracts a commented-out compilation or execution command, and runs it automatically.

This allows you to store the specific build instructions for a file *inside* the file itself, eliminating the need for complex Makefiles or repetitive terminal commands for small projects.

---

## Security Warning

> **IMPORTANT:** This utility uses the `system()` function to execute commands directly from source files. Running `ShebangWin` on untrusted files is dangerous, as a malicious user could hide harmful commands in the first line. Always inspect the source code of a file before using this utility on it.

---

## Features

* **Multi-Language Support**: Pre-configured for C, C++, Java, Python, Ruby, Lua, SQL, Haskell, and more.
* **Smart Extension Matching**: Automatically detects the correct comment syntax (e.g., `//`, `#`, `--`, `REM`).
* **Flexible Input**: Call it with the full filename (`compile main.cpp`) or just the base name (`compile main`).
* **Zero Dependencies**: A tiny, standalone C executable using the Win32 API.

---

## How to Build

### Option 1: Using the Build Script
If you have GCC installed, simply run the included batch file:
```cmd
build.bat
```

### Option 2: Manual Compilation
Alternatively, you can compile the utility manually using GCC (MinGW):
```bash
gcc -o compile main.c
```

*Note: For the best experience, move `compile.exe` to a folder in your System PATH.*

---

## Usage

1. **Add the command** to the very first line of your source file. It must be a single-line comment.

**Example (`main.cpp`):**
```cpp
// g++ -o main main.cpp && main.exe
#include <iostream>

int main() {
    std::cout << "Hello from ShebangWin!" << std::endl;
    return 0;
}
```

2. **Run the utility** from your command prompt or PowerShell:
```bash
compile main.cpp
```
*Or simply:*
```bash
compile main
```

---

## Supported Languages

| Extension | Comment Prefix |
| :--- | :--- |
| `.c`, `.cpp`, `.java` | `//` |
| `.py`, `.sh`, `.rb`, `.ps1` | `#` |
| `.lua`, `.sql`, `.hs` | `--` |
| `.bat` | `REM` |

> **Note:** You can easily add more languages by updating the `SUPPORTED_LANGUAGES` array in `main.c`.

---

## License

This project is licensed under the MIT License. Use it as you wish, but be mindful of the security implications.