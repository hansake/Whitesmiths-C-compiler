/	imul
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	mul int into int
/	stack: P, bc, hl, pc, R, L
/
/	P = 0
/	while (R)
/		if (R & 1)
/			P =+ L
/		L =<< 1
/		R =>> 1
/	L = P

	public	c.imul
R := 8
L := 10
c.imul:
	sp <= hl <= bc <= (hl = 0)
.1:
	a = *(hl = R[1] + sp) | *(hl - 1) jz .3
	a = *hl <^> -1 jnc .2
	bc =^ (hl = L + sp)
	hl <= sp + bc => sp
.2:
	a = *(hl = L + sp) + a -> *hl = *(hl + 1) +^ a -> *hl
	a = *(hl = R[1] + sp) <-> -1 -> *hl
	a = *(hl - 1) <^> -1 -> *hl
	jmp .1
.3:
	bc <= sp ->^ (hl = L[-2] + sp)
	jmp c.lret
