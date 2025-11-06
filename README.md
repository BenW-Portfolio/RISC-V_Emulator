# RISC-V_Emulator

RISC-V Instruction Set Simulator Project Summary

This project is a comprehensive, multi-phase RISC-V (RV64I) instruction set simulator built from the ground up in C. It emulates the core functionalities of a modern processor, starting with parsing assembly instructions and progressively implementing a register file, memory storage, control flow logic, and a direct-mapped cache. The simulator can execute RISC-V assembly programs by accurately modeling register manipulation, memory load/store operations, and conditional branching, providing a detailed look into the low-level execution of computer architecture.

    Technologies: C, Assembly

Project Goals and Phased Implementation:
This simulator was developed incrementally across five distinct phases, each building upon the last to create a fully functional model of a RISC-V processor.

Phase 1: Instruction Parsing & ISA Representation

The initial phase focuses on understanding and parsing the RISC-V instruction set. The simulator reads textual .archobj files, which contain a program header and a sequence of assembly instructions. A robust parser was implemented in C to convert each line of assembly into a structured C struct. This creates an in-memory representation of the program that can be used by subsequent emulation phases.

    Supported Instruction Categories:

        Register Instructions: Operations like add, sub, sll, ori, etc.

        Storage Instructions: Memory access operations like lb, sw, sd, etc.

        Control Flow Instructions: Branching and jumps like beq, jal, jalr, etc.

Phase 2: Register File and Arithmetic Emulation

This phase implements the core of the CPU: the register file. An array represents the 32 general-purpose RISC-V registers (x0-x31), along with the Program Counter (PC). The simulator single-steps through the parsed instructions, executing all register-based arithmetic and logical operations (add, addi, sub, sll, xor, etc.). It accurately tracks state changes, updating destination registers and advancing the PC after each instruction.

Phase 3: Memory and Storage System

To support real programs, a memory storage interface was developed. This phase emulates a block of main memory that the program can read from and write to.

    Implemented the full set of RISC-V storage instructions, including:

        Store Operations: sb, sh, sw, sd

        Load Operations: lb, lbu, lh, lhu, lw, lwu, ld

    Handles memory access of different sizes (1, 2, 4, and 8 bytes) and correctly performs sign-extension for signed load operations.

Phase 4: Control Flow and Branching Logic

This phase enables complex program logic by implementing control flow instructions. The simulator can now execute conditional branches and unconditional jumps, altering the Program Counter to change the flow of execution.

    Implemented Logic:

        Conditional Branches: beq, bne, blt, bge, etc., which jump to a target address based on register comparisons.

        Conditional Sets: slt, slti, which set a register to 0 or 1 based on a comparison.

        Unconditional Jumps: jal and jalr, which are essential for implementing function calls and returns.

Phase 5: Direct-Mapped Cache Simulation

The final phase adds a layer of performance modeling by simulating a direct-mapped cache. This component intercepts memory access requests from the load/store instructions. The cache tracks memory accesses to determine if they result in a hit or a miss, providing statistics on cache performance upon program completion.

    Models key cache behaviors such as cold misses, spatial/temporal locality, and conflict misses due to address mapping and eviction.
