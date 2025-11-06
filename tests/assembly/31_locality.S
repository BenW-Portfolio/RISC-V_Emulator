/*
 * Copyright (c) Son Nguyen, 2025.
 * Do not copy nor distribute without permission.
 */

/* Align to 64 bytes (2^6) */
.section .bss
.global memory
.align 6 
memory: .zero 4096

.section .text
.global _start
_start:
    lui s1, %hi(memory)
    addi s1, s1, %lo(memory)

    /* We will store these values into memory */
    li a0, 0x1
    li a1, 0x2
    li a2, 0x3

    /* Store values in addresses within 64 bytes of each other */
    sh a0, 0(s1)
    sh a1, 4(s1)
    sh a2, 8(s1)

    /* Exit */
    li a7, 0x2
    ecall