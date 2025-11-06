0:	auipc x9,0x1
8:	addi x10,x0,8
c:	ld x10,0(x9)
10:	addi x11,x0,42
14:	sd x11,64(x9)
18:	addi x12,x0,68
1c:	sd x12,128(x9)
20:	addi x17,x0,2
24:	ecall
