/*
 * Copyright (c) Oliver Krisetya, 2025.
 * Do not copy nor distribute without permission.
 */

.section .bss
.global memory
memory: .zero 256

.section .text
.global _start
_start:
    /* This test writes to the same set of cache-line-aligned memory addresses twice.
    *
    * First, we perform a loop of 3 writes to 3 distinct cache lines,
    * each spaced 64 bytes apart, writing into the cache lines.
    *
    * Then, we reset and write to the exact same memory locations again to simulate misses.
    */

    lui s1, %hi(memory)
    addi s1, s1, %lo(memory)

    addi s2, s1, 0

    /* a1 = NLINES */
    /* t0 = i */
    li a1, 3
    li t0, 0

store_loop:
    sw t0, 0(s2)
    addi s2, s2, 64

    addi t0, t0, 1
    bne a1, t0, store_loop

    addi s2, s1, 0
    li t0, 0

load_loop:
    lw t0, 0(s2)
    addi s2, s2, 64

    addi t0, t0, 1
    bne a1, t0, load_loop

    li a7, 0x2
    ecall