# cplusplus-from-first-principles

### C++ source code to runnable file works in four stages:

pre-processing (o/p: .i file) -> compilation (o/p: .s file) -> assembly (o/p: .o file) -> linking (o/p: executable) -> run

1. **Preprocessing**: The preprocessor handles directives (lines starting with `#`) — expanding macros, including header files, and stripping comments. Output is a `.i` file (plain C++ with all directives resolved).

2. **Compilation**: The compiler takes the preprocessed `.i` file and translates it into assembly language specific to the target platform. This stage checks for syntax and type errors. Output is a `.s` file (human-readable assembly).

3. **Assembly**: The assembler takes the `.s` file and translates assembly instructions into machine code. Output is a `.o` object file (binary, but not yet a complete program).

4. **Linking**: The linker takes one or more `.o` object files and combines them with any necessary libraries to produce the final executable. It resolves references between files (e.g. a function defined in one `.o` and called in another).

### Inspect each stage manually

**1. Preprocessing only — produces `hello.i`**

```bash
clang++ -E 01-Basics/hello.cpp -o hello.i
```

| Part                  | Meaning                                              |
| --------------------- | ---------------------------------------------------- |
| `clang++`             | The C++ compiler driver (manages all stages)         |
| `-E`                  | Stop after preprocessing; do not compile or assemble |
| `01-Basics/hello.cpp` | Input source file                                    |
| `-o hello.i`          | Write output to `hello.i` (the preprocessed source)  |

---

**2. Compilation only — produces `hello.s`**

```bash
clang++ -S hello.i -o hello.s
```

| Part         | Meaning                                                  |
| ------------ | -------------------------------------------------------- |
| `clang++`    | The C++ compiler driver                                  |
| `-S`         | Stop after compilation; output assembly, do not assemble |
| `hello.i`    | Input preprocessed source file from stage 1              |
| `-o hello.s` | Write output to `hello.s` (human-readable assembly)      |

---

**3. Assembly only — produces `hello.o`**

```bash
clang++ -c hello.s -o hello.o
```

| Part         | Meaning                                                             |
| ------------ | ------------------------------------------------------------------- |
| `clang++`    | The C++ compiler driver (invokes the assembler `as` internally)     |
| `-c`         | Stop after assembling; produce object file, do not link             |
| `hello.s`    | Input assembly file from stage 2                                    |
| `-o hello.o` | Write output to `hello.o` (binary machine code, not yet executable) |

---

**4. Linking — produces the executable `hello`**

```bash
clang++ hello.o -o hello
```

| Part       | Meaning                                                       |
| ---------- | ------------------------------------------------------------- |
| `clang++`  | The C++ compiler driver (invokes the linker `ld` internally)  |
| `hello.o`  | Input object file from stage 3 (can pass multiple `.o` files) |
| `-o hello` | Write output to `hello` (the final executable)                |

---

**5. Run**

```bash
./hello
```

| Part    | Meaning                                                                                               |
| ------- | ----------------------------------------------------------------------------------------------------- |
| `./`    | Run from the current directory (the OS won't find it otherwise since `.` is not in `PATH` by default) |
| `hello` | The executable produced by the linker                                                                 |

| Stage | Input | Output | Flag |
|---------------|-----------|---------|-----------|
| Preprocessing | hello.cpp | hello.i | `-E` |
| Compilation | hello.i | hello.s | `-S` |
| Assembly | hello.s | hello.o | `-c` |
| Linking | hello.o | hello | (default) |

### One-shot compile and run

```bash
clang++ 01-Basics/hello.cpp -std=c++20 -o hello && ./hello
```
