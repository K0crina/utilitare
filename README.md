# Linux Utilities

Four command-line utilities implemented from scratch in C that replicate the core behavior of classic Unix tools (`ls`, `cd`, `diff`), plus a standalone dependency analyzer for C/C++ codebases. The focus is on low-level argument parsing, dynamic memory management, recursive directory traversal and edit-distance computation.

---

## Build

```bash
# Main binary (Tasks 1–4)
gcc tema.c task1.c task2.c task3.c task4.c -o tema

# Standalone dependency checker (Bonus)
gcc util_lib.c -o util_lib
```

---

## Tasks

### Task 1 — Argument Parser

Parses a command-line call into three categories of parameters: positional **arguments**, **options** (short `-f val` / long `--file=val`, with a value), and **flags** (short `-v` / long `--verbose`, no value). The expected parameter schema is read from an input file; the parsed result is written to an output file.

The parser handles combined short flags (`-rh`), optional next-token values for options, and both `=` and space-separated syntax.

```bash
./tema task1 --file=input.in --out=output.out [positional] [--opt=val] [-f]
```

**Key data structures:** `argument` struct with a `tip` field (`'a'`/`'o'`/`'f'`), a dynamic `ID[]` array for short/long aliases, and a `void* valoare` pointer.

---

### Task 2 — `ls` Simulation

Recursively traverses a directory using `opendir`/`readdir` and builds an in-memory stack of `Node` structs (one per directory), each holding a linked list of its `File` entries with sizes from `stat()`. Entries are sorted alphabetically using `qsort`.

Supports four display modes:

| Flag | Behavior |
|---|---|
| *(none)* | Non-recursive listing, raw byte sizes |
| `-h` | Non-recursive, sizes converted to KB |
| `-r` | Recursive listing |
| `-rh` | Recursive + human-readable sizes |

```bash
./tema task2 <directory_path> [-h|-r|-rh] --out=output.out
```

---

### Task 3 — Relative Path Calculator (`cd`)

Given two absolute paths, computes the relative path from one to the other — the same navigation sequence that `cd` would require.

The algorithm:
1. Splits both paths into components and finds the deepest common ancestor directory.
2. Traverses the filesystem from that ancestor using a stack (same approach as Task 2) and builds a **tree** of `treeNode` structs.
3. Runs a DFS from the root to find both the source and destination nodes, then constructs the relative path: `../../subdir/target`.

```bash
./tema task3 /source/path /destination/path --out=output.out
```

---

### Task 4 — Line-by-Line `diff`

Compares two files line by line using a character-level edit distance algorithm. For each pair of lines (same index), it computes the minimum number of:

- **D** — deletions (characters in `ref` not in `compare`)
- **I** — insertions (characters in `compare` not in `ref`)
- **R** — replacements (characters that differ in place)

If the files have different lengths, the shorter one is padded with empty lines.

```bash
./tema task4 file1.txt file2.txt --out=output.txt
```

Output format (one line per pair):
```
2D 1I 0R
```

---

### Bonus — Include Dependency Checker

A standalone utility (`util_lib.c`) that scans a directory for `.c` and `.h` files, parses their `#include` directives, and detects two classes of architectural problems:

- **Circular includes** — files that include each other, directly or transitively (A → B → A), detected via DFS cycle detection.
- **Double includes** — the same header included redundantly across multiple files (reported with `--full`).

```bash
./util_lib <directory_path> [--full]
```

---

## Project Structure

```
.
├── tema.c          # Entry point — dispatches to task1–4 based on argv[1]
├── task1.c / .h    # Argument parser
├── task2.c / .h    # ls simulation — stack + linked-list per directory
├── task3.c / .h    # cd path calculator — stack → tree → DFS
├── task4.c / .h    # diff — line reader + edit distance
├── util_lib.c      # Standalone include dependency checker
├── teste/          # Input test files
└── ref/            # Reference outputs for all tests
```

---

## Testing

Reference outputs for all 20 test cases are in `ref/`. To verify a task:

```bash
./tema task2 teste/dir1 -r --out=my_out.txt
diff my_out.txt ref/data3.ref
```

The `comenzi.sh` script contains the full set of invocations used during grading.
