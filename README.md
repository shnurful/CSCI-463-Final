# CSCI-463-Final — RV32I Simulator

A RISC-V (RV32I) instruction set simulator written in C++. It loads a binary
executable into a simulated memory, disassembles the machine code into readable
assembly, and executes it one instruction at a time — tracking registers,
program counter, and memory state along the way.

This was built as the multi-part final for CSCI 463 at NIU, assembled in three
stages: a memory simulator, an RV32I disassembler, and finally a full
instruction-executing simulator.

## Features

- **Simulated memory** of configurable size, loaded directly from a binary file
- **Hex-dump output** of memory contents in the classic `offset: bytes *ascii*` format
- **Disassembler** that decodes 32-bit words into RV32I assembly
- **Instruction execution** with a single hart (hardware thread), including a
  32-entry register file where `x0` is hardwired to zero
- **Execution tracing** — optionally print each instruction and/or the full
  register/PC state as the program runs
- **Halt handling** that stops on `ebreak`, an illegal instruction, or a
  user-supplied instruction-count limit, and reports why it stopped

## Project structure

| File | Responsibility |
|------|----------------|
| `main.cpp` | Entry point: parses arguments, sets up memory, runs disassembly/simulation |
| `hex.h` / `hex.cpp` | Hex formatting helpers (`to_hex32`, `to_hex0x32`, etc.) |
| `memory.h` / `memory.cpp` | Simulated memory: file loading, hex dump, address checks, 8/16/32-bit access |
| `rv32i_decode.h` / `rv32i_decode.cpp` | Instruction decoding and disassembly rendering |
| `registerfile.h` / `registerfile.cpp` | The 32 general-purpose registers (x0–x31) |
| `rv32i_hart.h` / `rv32i_hart.cpp` | A single hart: fetch/decode/execute, PC, halt state |
| `cpu_single_hart.h` / `cpu_single_hart.cpp` | Drives one hart through the run loop |

## Building

The project ships with a `Makefile`, so from the project directory just run:

```sh
make
```

That compiles every `.cpp` in the directory with `g++` under C++14
(`-g -ansi -pedantic -Wall -Werror -Wextra`) and links them into the `rv32i`
executable. Header dependencies are tracked automatically (`-MMD -MP`), so
editing a header rebuilds only what needs it.

Other targets:

| Command | Effect |
|---------|--------|
| `make` / `make all` | Build the `rv32i` executable |
| `make clean` | Remove the executable and all generated `.o` / `.d` files |
| `make re` | Clean and rebuild from scratch |

## Usage

```
rv32i [-d] [-i] [-r] [-z] [-l exec-limit] [-m hex-mem-size] infile
```

| Option | Effect |
|--------|--------|
| `-d` | Show a disassembly of memory before execution begins |
| `-i` | Print each instruction as it executes |
| `-r` | Dump the registers and PC before each instruction |
| `-z` | Dump register and memory state after the simulation halts |
| `-l exec-limit` | Max number of instructions to execute (`0` = no limit; default) |
| `-m hex-mem-size` | Memory size in hex (default `0x100`) |
| `infile` | The binary file to load and run |

Flags may be given separately or bundled — `-d -i -r` and `-dir` are equivalent,
and short-option arguments can be attached (`-m100`, `-l2`).

### Example

```sh
./rv32i -m100 -irl2 allinsns5.bin
```

Runs `allinsns5.bin` in a `0x100`-byte memory, printing instructions (`-i`) and
register dumps (`-r`), stopping after 2 instructions (`-l2`):

```
x0  00000000 f0f0f0f0 00000100 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
...
pc  00000000
00000000: abcde237  lui   x4,0xabcde        // x4 = 0xabcde000
...
2 instructions executed
```

## Author

Ethan Silo — CSCI 463, Northern Illinois University.

Built on starter code provided for the assignment; extended where appropriate.
