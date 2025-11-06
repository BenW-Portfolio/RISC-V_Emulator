/*
 * Copyright (c) Oliver Krisetya, 2025.
 * Do not copy nor distribute without permission.
 */

.section .bss
.global memory
memory: .zero 4096

.section .text
.global _start
_start:
    /* This test performs 8 store instructions, each to a new cache-line-aligned
    * memory address. The stride is 64 bytes, which matches a typical cache line size.
    *
    * Every access is to a unique address (no reuse), so this test simulates
    * a worst-case cold-load scenario.
    *
    *  - If the cache has ≥ 8 lines: All accesses are cold misses (first-time accesses),
    *    but no evictions occur.
    *  - If the cache has < 8 lines: After the cache fills up, accesses will start
    *    evicting earlier lines. There will be conflict/capacity misses.
    * In both cases, we will have 100% miss rate and 0% hit rate.
    */

    lui s1, %hi(memory)
    addi s1, s1, %lo(memory)

    /* a1 = NLINES */
    /* t0 = i */
    li a1, 8
    li t0, 0

loop:
    sw t0, 0(s1)
    addi s1, s1, 64

    addi t0, t0, 1
    bne a1, t0, loop

    li a7, 0x2
    ecall