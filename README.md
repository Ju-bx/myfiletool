# MyFileTool

A small C++17 command-line tool for Linux that counts lines, words, and characters in text files, and reports the most frequent words.

Built as a learning project to practice modern C++, CMake, and Linux development tools.

## Features

- Line, word, and character counts (results match `wc`)
- Word frequency with normalization (lowercase, punctuation removed)
- Top-N most frequent words with `--top N`
- Multiple files, with a combined total
- Clear error messages and exit codes

## Build

Requirements: g++ with C++17 support, CMake 3.16 or later.

```bash
git clone git@github.com:Ju-bx/myfiletool.git
cd myfiletool
cmake -S . -B build
cmake --build build
```

## Usage

```bash
./build/myfiletool [--top N] <file>...
```

| Option | Description |
|---|---|
| `--top N` | Show the N most frequent words (default: 5) |

### Example

```bash
./build/myfiletool --top 2 test/sample.txt test/top.txt
```

```
File: test/sample.txt
Lines: 5
Words: 26
Characters: 150

Top 2 words:
and: 2
data: 2
File: test/top.txt
Lines: 1
Words: 9
Characters: 55

Top 2 words:
zebra: 4
mango: 3
=========total=========
Lines: 6
Words: 35
Characters: 205

Top 2 words:
zebra: 4
mango: 3
```

### Exit codes

| Code | Meaning |
|---|---|
| `0` | Success |
| `1` | Invalid arguments, or at least one file could not be opened |

If a file cannot be opened, the tool reports the error and continues with the remaining files.

## Design notes

**Top-N in a single pass.** Instead of sorting all words (O(n log n)), the tool keeps a sorted list of at most N entries. Each new word is compared with the smallest entry and, if larger, moved forward into place, like one step of insertion sort. This runs in O(n · N), which is faster when N is small.

**Tie-breaking.** Words are stored in a `std::map`, which iterates in alphabetical order. Because a word only replaces another when its count is strictly greater, words with equal counts stay in alphabetical order.

**Reusable components.** Each file's statistics are stored in a `FileStats` struct. The total is just another `FileStats` built by merging, so the same `top_words` and `print_report` functions work for both single files and the total.

## Development

### Build modes

The project uses two separate build directories, each configured once:

| Directory | Purpose | Configure command |
|---|---|---|
| `build/` | Normal debug build (also used with GDB) | `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug` |
| `build-asan/` | AddressSanitizer build for memory checks | see below |

```bash
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer"
```

After changing the code, rebuild the directory you want to use:

```bash
cmake --build build        # normal
cmake --build build-asan   # AddressSanitizer
```

### Testing

Results are checked against `wc`:

```bash
./build/myfiletool test/sample.txt test/top.txt
wc test/sample.txt test/top.txt
```

### Checking memory errors with AddressSanitizer

```bash
cmake --build build-asan
./build-asan/myfiletool test/sample.txt
```

No output from ASan means no memory errors were detected in that run. If an error is found, the report shows:

1. The error type, such as `heap-buffer-overflow`, and whether it was a READ or WRITE
2. The source line where it happened (the first `#0` frame in your own code)
3. How far out of bounds the access was (`located N bytes before/after ...`)
4. Where the memory was allocated (`allocated by thread ...`)

### Debugging with GDB

```bash
cmake --build build
gdb --args ./build/myfiletool --top 3 test/top.txt
```

| Command | Action |
|---|---|
| `break top_words` / `break main.cpp:113` | Set a breakpoint |
| `run` | Start the program |
| `next` / `step` | Next line / step into a function |
| `print top` / `display top` | Show a variable / show it at every stop |
| `info args` / `info locals` | Show arguments / local variables |
| `bt` | Show the call stack |
| `continue` / `quit` | Continue / exit |

### Which tool to use

| Problem | Tool |
|---|---|
| Wrong result | GDB: break near the suspect code and inspect variables |
| Crash (segmentation fault) | ASan first; if it finds nothing, run in GDB and use `bt` |
| Program hangs | GDB: press `Ctrl + C`, then `bt` |
| Everything looks fine | Run the tests under ASan anyway |

## Known limitations

- Words are separated by spaces only; tabs are not treated as separators
- The character count assumes every line ends with a newline
- Punctuation is removed, so `C++` becomes `c` and `don't` becomes `dont`
- Only ASCII text is handled correctly
- `--top 3abc` is accepted as `3`, because `std::stoi` stops at the first non-digit
- Only arguments containing `.txt` are treated as files

## Project structure

```
myfiletool/
├── CMakeLists.txt
├── src/
│   └── main.cpp
├── test/
│   ├── sample.txt
│   └── top.txt
└── experiments/
    └── asan_demo.cpp
```