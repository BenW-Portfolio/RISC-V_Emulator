/*
 * Copyright (c) Son Nguyen, 2025.
 * Do not copy nor distribute without permission.
 */

 /* Align to 64 bytes (2^6) */
.section .bss
.global memory
.align 6
memory: .zero 65536

.section .text
.global _start
_start:
    /* If we alternate memory accesses at a0 and a1, where a1 is a multiple 
     * of a0 such that it maps to the same cache line, then we should always
     * get a miss for every access since we must evict the previous value.
     * Here we assume that there's only at most 8 cache lines. 
     */
    lui s1, %hi(memory)
    addi s1, s1, %lo(memory)

    /* We will store these values into memory */
    li a0, 0x1
    li a1, 0x2
    li a2, 0x3

    /* Store values in addresses that are offsetted by a multiple of 64*8 (cache line size * cache line) */
    sh a0, 0(s1)
    sh a1, 512(s1)
    sh a2, 1024(s1)

    /* Exit */
    li a7, 0x2
    ecall