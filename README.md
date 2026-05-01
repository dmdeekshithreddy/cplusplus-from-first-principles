# cplusplus-from-first-principles

### C++ source code to runnable file works in four stages:

pre-processing (o/p: .ii file) -> compilation (o/p: .s file) -> assembly (o/p: .o file) -> linking (o/p: executable) -> run

`clang++` is a **driver** — it does not do the work itself. It figures out which stage you want and delegates to the right underlying tool.

| Stage | Underlying tool | Language-aware? |
| ------------- | --------------- | --------------- |
| Preprocessing | `clang` (preprocessor) | Yes |
| Compilation | `clang` (compiler frontend) | Yes |
| Assembly | `as` (assembler) | No |
| Linking | `ld` (linker) | No |

`as` and `ld` are language-agnostic — the same tools are used for C, C++, Rust, and any other compiled language. By the time they run, all language-specific work is already done:
- `as` just sees architecture-specific assembly instructions, not C++ syntax
- `ld` just sees binary object files and symbol names

The only difference between `clang` and `clang++` is in the compiler frontend — `clang++` enables C++ parsing, name mangling, and template instantiation. After stage 2, the output is plain assembly that `as` and `ld` treat identically regardless of source language.

1. **Preprocessing** (`clang`): Handles directives (lines starting with `#`) — expanding macros, including header files, and stripping comments. Output is a `.ii` file (plain C++ with all directives resolved).

2. **Compilation** (`clang`): Takes the preprocessed `.ii` file and translates it into assembly language specific to the target platform. This stage checks for syntax and type errors. Output is a `.s` file (human-readable assembly).

3. **Assembly** (`as`): Takes the `.s` file and translates human-readable assembly instructions into binary machine code. Output is a `.o` object file (binary, but not yet a complete program).

4. **Linking** (`ld`): Takes one or more `.o` object files and combines them with any necessary libraries to produce the final executable. It resolves references between files (e.g. a function defined in one `.o` and called in another).

### Inspect each stage manually

**1. Preprocessing only — produces `hello.ii`**

```bash
clang++ -E 01-Basics/hello.cpp -o hello.ii
```

| Part                  | Meaning                                                                                                 |
| --------------------- | ------------------------------------------------------------------------------------------------------- |
| `clang++`             | The C++ compiler driver (manages all stages)                                                            |
| `-E`                  | Only run the preprocessor; do not compile or assemble                                                   |
| `01-Basics/hello.cpp` | Input source file                                                                                       |
| `-o hello.ii`          | Write output to `hello.ii` (the preprocessed source) — optional; if omitted, output is printed to stdout |

> **Note:** Use `.ii` (not `.i`) for C++ preprocessed files. `.i` is the C extension — passing it to `clang++` triggers a deprecation warning. `.ii` tells the compiler the input is C++ preprocessed output.
>
> The `.ii` file is dramatically larger than the source. For a simple `hello.cpp` (~4 KB), the preprocessed `hello.ii` is ~2.6 MB — because `#include <iostream>` causes the entire iostream header chain to be copied in verbatim.

---

**2. Compilation only — produces `hello.s`**

```bash
clang++ -S hello.ii -o hello.s
```

| Part         | Meaning                                                                                                                        |
| ------------ | ------------------------------------------------------------------------------------------------------------------------------ |
| `clang++`    | The C++ compiler driver                                                                                                        |
| `-S`         | Only run preprocess and compilation steps; output assembly, do not assemble                                                    |
| `hello.ii`    | Input preprocessed source file from stage 1                                                                                    |
| `-o hello.s` | Write output to `hello.s` (human-readable assembly) — optional; if omitted, defaults to the input basename with `.s` extension |

> **Note:** The `.s` file is much smaller than `hello.ii` (~52 KB vs ~2.6 MB). The `hello.ii` file contains thousands of lines of template definitions, type declarations, and inline functions from `<iostream>`, but the compiler only emits assembly for code that is actually instantiated and called. Everything unused is discarded at this stage.

---

**3. Assembly only — produces `hello.o`**

```bash
clang++ -c hello.s -o hello.o
```

| Part         | Meaning                                                                                                                                        |
| ------------ | ---------------------------------------------------------------------------------------------------------------------------------------------- |
| `clang++`    | The C++ compiler driver (invokes the assembler `as` internally)                                                                                |
| `-c`         | Stop after assembling; produce object file, do not link                                                                                        |
| `hello.s`    | Input assembly file from stage 2                                                                                                               |
| `-o hello.o` | Write output to `hello.o` (binary machine code, not yet executable) — optional; if omitted, defaults to the input basename with `.o` extension |

> **Note:** The `.o` file shrinks from ~52 KB to ~12 KB compared to `hello.s`. The assembly file stores instructions as human-readable text (e.g. `movq`, `callq`, label names) — the assembler encodes those as compact binary opcodes, which are significantly smaller.

---

**4. Linking — produces the executable `hello`**

```bash
clang++ hello.o -o hello
```

| Part       | Meaning                                                                                    |
| ---------- | ------------------------------------------------------------------------------------------ |
| `clang++`  | The C++ compiler driver (invokes the linker `ld` internally)                               |
| `hello.o`  | Input object file from stage 3 (can pass multiple `.o` files)                              |
| `-o hello` | Write output to `hello` (the final executable) — optional; if omitted, defaults to `a.out` |

---

**5. Run**

```bash
./hello
```

| Part    | Meaning                                                                                               |
| ------- | ----------------------------------------------------------------------------------------------------- |
| `./`    | Run from the current directory (the OS won't find it otherwise since `.` is not in `PATH` by default) |
| `hello` | The executable produced by the linker                                                                 |

### Summary

| Stage         | Input     | Output  | Flag      |
| ------------- | --------- | ------- | --------- |
| Preprocessing | hello.cpp | hello.ii | `-E`      |
| Compilation   | hello.ii   | hello.s | `-S`      |
| Assembly      | hello.s   | hello.o | `-c`      |
| Linking       | hello.o   | hello   | (default) |

### File size journey (simple hello.cpp)

| File       | Size    | Why |
| ---------- | ------- | --- |
| `hello.cpp`  | ~4 KB   | Your source code |
| `hello.ii`   | ~2.6 MB | All headers copied in verbatim — no filtering |
| `hello.s`    | ~52 KB  | Only code actually used, compiled to assembly |
| `hello.o`    | ~12 KB  | Same code as `.s` but binary-encoded — text representations of instructions and symbols compress down significantly |
| `hello`      | varies  | Object code + runtime startup code linked in from stdlib |

### One-shot compile and run

```bash
clang++ 01-Basics/hello.cpp -std=c++20 -o hello && ./hello
```
