/*
 * Copyright (c) Son Nguyen, 2025.
 * Do not copy nor distribute without permission.
 */

.section .bss
.global memory
memory: .zero 4096

.section .text
.global _start
_start:
    /* We perform multiple accesses here to write to memory.
     * For the first NACCESSSES, if we have at least NLINES in our actual cache,
     * we should get a 1/NACCESSES miss rate. (If NACCESSES = 3, then 66% hit rate).
     * Total misses should only equal NLINES in this case.
     * If we have less than NLINES, we should get a lower hit rate due to eviction. 
     * Run this test with different values of NLINES to see the effect.
     */
    

    lui s1, %hi(memory)
    addi s1, s1, %lo(memory)
    li s2, 0x3456789abcdef012

    /* a0 = NACCESSES */
    /* a1 = NLINES */
    /* t0 = k (Outer loop)*/
    /* t1 = i (Inner loop) */
    addi a0, x0, 3
    addi a1, x0, 8

    addi t0, x0, 0
outerLoop:
    /* Reset i and memory address */
    addi t1, x0, 0
    add s3, x0, s1
innerLoop:
    /* Access memory, then we increment s2 (memory address) by 64 bytes because that's the size of cache line in x86 */
    sd s4, 0(s3)
    addi s3, s3, 64
    
    /* Wrote to this cacheline, increment i to write to the next cacheline */
    addi t1, t1, 1
    blt t1, a1, innerLoop

    /* We wrote to NLINES different cachelines, increment i and reset so we write to same location again.
     * On iterations after the first one, if we have enough cache lines, we should get hits.
     */
    addi t0, t0, 1
    blt t0, a0, outerLoop

    /* Exit */
    li a7, 0x2
    ecall