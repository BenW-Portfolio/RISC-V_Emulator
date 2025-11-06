/*
 * Copyright (c) Son Nguyen, 2024.
 * Do not copy nor distribute without permission.
 */

/* Allocate 4096 bytes for the execution stack */
.section .bss
.global global_variable
global_variable: .zero 4096

/* The starting instructions that call main */
.section .text
.global _start
_start:
	/* Write to different cache lines with different offset from each other 
	 * to ensure 100% miss rate. We assume that we have at least 4 cache lines.
	 */
	

	la s1, global_variable
	
	/* First cache-line */
	li a0, 8
	ld a0, 0(s1) 

	/* Second cache-line, exactly 64 bytes away. 100% miss rate here. */
	li a1, 42	
	sd a1, 64(s1) 

	/* Third cache-line, exactly 128 bytes away. 100% miss rate here. */
	li a2, 68	
	sd a2, 128(s1) 

	/* Exit */
	li a7, 0x2
	ecall
