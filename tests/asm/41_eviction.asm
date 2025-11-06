0:	lui x9,0x1
8:	addi x10,x0,1
c:	addi x11,x0,2
10:	addi x12,x0,3
14:	sh x10,0(x9)
18:	sh x11,512(x9)
1c:	sh x12,1024(x9)
20:	addi x17,x0,2
24:	ecall
