📁 Project: Terminal Utilities

📌 General Description
This project consists of developing four command-line utilities that emulate the behavior of classic Linux commands (`ls`, `diff`, `cd`). 
The focus is on parameter processing, directory structure management, relative path calculation, and identifying differences between files.

▶️ Compilation
- For the main application (Tasks 1-4): 
gcc tema.c task1.c task2.c task3.c task4.c -o tema

- For the dependency checker utility (Bonus):
gcc util_lib.c -o util_lib


📂 Task 1 – Argument and Option Parsing

📘 Description:
Imitates the behavior of variadic terminal commands. It receives a call like `./program options arguments flags` and identifies:
- Positional arguments
- Options (short/long)
- Flags (no value)

🧪 Usage:
./tema task1 --file=input.in --out=output.out [arguments] [--opt=val] [-f]

📂 Task 2 – `ls` Simulation with Stack

📘 Description:
Simulates the `ls` command and displays the contents of a directory, supporting:
- -h / --human-readable
- -r / --recursive
- -d / --directory

🧪 Usage:
./tema task2 <directory_path> [d|h|r|rh] --out=output.out

📂 Task 3 – Relative Path Calculation between Directories

📘 Description:
Imitates the `cd` command for navigating the directory tree. It builds a tree and determines the relative path between two absolute directories.

🧪 Usage:
./tema task3 /source/path /destination/path --out=output.out

📂 Task 4 – File Comparison (Line-by-Line Diff)

📘 Description:
Simulates the `diff` command. It compares two files line by line and calculates:
- Delete (D)
- Insert (I)
- Replace (R)

🧪 Usage:
./tema task4 file1.txt file2.txt --out=output.txt

📂 Bonus – Dependency Checker (Includes)

📘 Description:
A separate utility (`util_lib.c`) that traverses a directory and analyzes C/C++ source files (`.c`, `.h`). 
It identifies potential architectural issues in the code by detecting include loops (circular dependencies) and redundant imports.
- Loop Detection: Detects if files mutually include each other in an infinite loop (e.g., A includes B, which includes A).
- Double Includes: Using the `--full` flag, it displays warnings (WARNING) if a specific header file is redundantly included multiple times across different files.

🧪 Usage:
./util_lib <directory_path> [--full]
