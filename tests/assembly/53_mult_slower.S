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
     * Here instead of writing an entire double-word into the cacheline, we 
     * write 1 byte at a time to simulate slower access.
     * This means that there are multiple values that thus can fit into the cache line.
     * We thus should get additional hits since additional accesses all go to the same cache line.
     * The only misses that should occur is when we first access a new cache line.
     */


    lui s1, %hi(memory)
    addi s1, s1, %lo(memory)
    li s2, 0x24

    /* a0 = NACCESSES */
    /* a1 = NLINES */
    /* a3 = 64 = cache line size */
    /* t0 = k (Outermost loop) */
    /* t1 = i (Middle loop) */
    /* t2 = j (Innermost loop) */
    addi a0, x0, 3
    addi a1, x0, 8
    addi a2, x0, 64

    addi t0, x0, 0
outerLoop:
    /* Reset i and memory address */
    addi t1, x0, 0
    add s3, x0, s1
innerLoop:
    /* Reset j */
    addi t2, x0, 0
innermostLoop:
    /* Access memory, then we increment s2 (memory address) by 1 byte. */
    sb s4, 0(s3)
    addi s3, s3, 1
    
    /* Wrote to one byte of this cacheline, increment j to continue writing to this cacheline */
    addi t2, t2, 1
    blt t2, a2, innermostLoop

    /* Finished writing to this cacheline, increment i to write to the next cacheline */
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