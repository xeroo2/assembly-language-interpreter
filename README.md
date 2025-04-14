Assembly Language Simulator (C++)
This project is a basic Assembly Language Instruction Simulator implemented in C++.
It reads a list of instructions from a file (instructions.asm), simulates them, manages registers, memory, and flags, and writes the final state into an output file (output.txt).


⚙️ How It Works
Reads instructions from instructions.asm.

Executes each instruction one by one.

Updates:

Registers (R0–R6)

Flags (Overflow, Underflow, Carry, Zero)

Memory (64 integer memory cells)

Program Counter (instruction count)

Outputs the final state to output.txt.


🛠️ Supported Instructions
Data Transfer
MOV, IN, OUT, STORE, LOAD

Arithmetic Operations
ADD, SUB, MUL, DIV, INC, DEC

Bitwise Operations
ROR (Rotate Right), ROL (Rotate Left), SHL (Shift Left), SHR (Shift Right)

Flags Management
Overflow (OF), Underflow (UF), Carry (CF), Zero (ZF)

Example instructions.asm
MOV R0 5

MOV R1 10

ADD R2 R0 R1

OUT R2

STORE R2 20

LOAD R3 20

MUL R3 2

OUT R3

📋 Notes
Input validation is included for IN instructions to prevent invalid entries.

Case-insensitive instruction parsing (e.g., add, ADD, Add all work).

Memory has 64 locations, initialized to 0.

Flags are updated after every arithmetic or shift operation.
