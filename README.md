# 📁 File Organizer Tool  
_Automated directory cleaner written in ANSI C_

```
      ______ _ _        ___                        _             
     |  ____(_) |      / _ \                      | |            
     | |__   _| | ___ | | | |_ __   ___ _ __   ___| |_ ___  _ __ 
     |  __| | | |/ _ \| | | | '_ \ / _ \ '_ \ / __| __/ _ \| '__|
     | |    | | |  __/| |_| | |_) |  __/ | | | (__| || (_) | |   
     |_|    |_|_|\___| \___/| .__/ \___|_| |_|\___|\__\___/|_|   
                             | |                                  
                             |_|                                  
```

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
![Language: C](https://img.shields.io/badge/Language-C-blue)
![Status: Stable](https://img.shields.io/badge/Status-Stable-brightgreen)
![Platform: Cross-Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey)
[![Author](https://img.shields.io/badge/Author-Mobin%20Yousefi-blue)](https://github.com/mobinyousefi-cs)

---

## 📌 Overview

**File Organizer Tool** is a lightweight, fast, and cross-platform directory organizer written in pure C.  
It scans a target directory, detects file types based on extensions, and automatically moves them into categorized subfolders (e.g., *Images*, *Documents*, *Audio*, *Archives*, etc.).

This project demonstrates:

- File & directory traversal  
- Pattern-based categorization  
- Safe file renaming logic  
- Modular C architecture (headers + sources)  
- Logging system with verbosity levels  

Perfect for learning or extending into a complete desktop utility.

---

## ✨ Features

- Categorizes files based on extension into folders:
  - `Images/`, `Documents/`, `Audio/`, `Video/`, `Spreadsheets/`,  
    `Presentations/`, `Archives/`, `Source/`, `Other/`
- Automatic subdirectory creation
- Safe renaming to avoid overwriting existing files
- Cross-platform (`Linux`, `macOS`, `Windows`)
- Dry-run mode (`--dry-run`)
- Verbose debugging (`--verbose`)
- Clean modular structure (professional MSc-level project)

---

## 📂 Directory Structure

```text
File-Organizer-Tool/
├── include/
│   ├── organizer.h
│   └── logger.h
├── src/
│   ├── main.c
│   ├── organizer.c
│   └── logger.c
├── tests/
│   └── README.md
├── build/      (auto-created)
├── bin/        (auto-created)
├── .github/
│   └── workflows/
│       └── ci.yml
├── Makefile
├── .gitignore
├── LICENSE
└── README.md
```

---

## ⚙️ Building the Project

### **Linux / macOS**
```bash
make
```

### **Windows (MinGW / MSYS2)**
```bash
mingw32-make
```

The compiled executable will be available at:

```text
bin/file_organizer
```

---

## ▶️ Running the Tool

### **Basic usage**
```bash
./bin/file_organizer
```

### **Target a specific directory**
```bash
./bin/file_organizer --dir /path/to/folder
```

### **Dry-run mode (no changes, only logs)**
```bash
./bin/file_organizer -n -d test_folder
```

### **Verbose debugging**
```bash
./bin/file_organizer -v
```

### **Full usage**
```text
Usage: file_organizer [options] [DIRECTORY]

Options:
  -d, --dir DIR     Target directory (default: current directory)
  -n, --dry-run     Show planned moves without changing the file system
  -v, --verbose     Enable verbose debugging
  -h, --help        Show this help message
```

---

## 📊 Example Output

```text
[2025-11-29 10:41:33] INFO : Organizing files in './Downloads'
[2025-11-29 10:41:33] INFO : Created directory: Downloads/Images
[2025-11-29 10:41:33] INFO : Moved 'photo.png' -> 'Downloads/Images/photo.png'
[2025-11-29 10:41:33] INFO : Created directory: Downloads/Documents
[2025-11-29 10:41:33] INFO : Moved 'report.pdf' -> 'Downloads/Documents/report.pdf'
```

---

## 🧩 Extending This Project

Some recommended extensions:

- Add a configuration file (JSON/YAML)
- Add a GUI using C/GTK or Qt
- Add multithreading for faster processing
- Add full recursive directory organization
- Add logging to a file

---

## 🧑‍💻 Author

**Mobin Yousefi**  
Master's in Computer Science  
GitHub: https://github.com/mobinyousefi-cs  

For academic collaboration, software projects, or research discussions, feel free to reach out.

---

## 📄 License

This project is licensed under the **MIT License**.  
You are free to use, modify, and distribute this project.
