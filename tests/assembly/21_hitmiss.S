/*
 * Copyright (c) Oliver Krisetya, 2025.
 * Do not copy nor distribute without permission.
 */
.section .bss
.global memory
memory: .zero 48

.section .text
.global _start
_start:
    lui s1, %hi(memory)
    addi s1, s1, %lo(memory)

    /* We will store these values into memory */
    li a0, 0x1
    li a1, 0x2
    li a2, 0x3
    
    /* Store values in the same location in memory. We should get a miss, then hits */
    sw a0, 0(s1)
    sw a1, 0(s1)
    sw a2, 0(s1)

    /* Exit */
    li a7, 0x2
    ecall