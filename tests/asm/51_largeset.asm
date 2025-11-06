0:	lui x9,0x1
8:	addi x11,x0,8
c:	addi x5,x0,0
10:	sw x5,0(x9)
14:	addi x9,x9,64
18:	addi x5,x5,1
1c:	bne x11,x5,10
20:	addi x17,x0,2
24:	ecall
