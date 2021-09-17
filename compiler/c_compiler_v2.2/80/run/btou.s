/	btou
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	convert bits to unsigned
/	stack: bc, hl, pc, offset/size, ptr to bits

	public	c.btou
O := 6
S := 7
P := 8
c.btou:
	sp <= hl <= bc
	bc =^ (hl = P + sp =a^ hl)
	a = *(hl = O + sp) -> l
.0:
	l - 1 jm .1
	a = b <^> -1 -> b = c <^> -1 -> c
	jmp .0
.1:
	a = *(hl = S + sp)
	hl = 1
.2:
	a - 1 jm .3
	hl + hl
	jmp .2
.3:
	hl - 1
	a = c & l -> c = b & h -> b
	bc ->^ (hl = P + sp)
	jmp c.lret
