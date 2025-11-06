0:	lui x9,0x1
8:	addi x18,x9,0
c:	addi x11,x0,3
10:	addi x5,x0,0
14:	sw x5,0(x18)
18:	addi x18,x18,64
1c:	addi x5,x5,1
20:	bne x11,x5,14
24:	addi x18,x9,0
28:	addi x5,x0,0
2c:	lw x5,0(x18)
30:	addi x18,x18,64
34:	addi x5,x5,1
38:	bne x11,x5,2c
3c:	addi x17,x0,2
40:	ecall
