/	ilsh
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	integer shifts
/	stack: bc, hl, pc, R, L

	public	c.ilsh
	public	c.irsh
	public	c.ursh
R := 6
L := 8
c.ilsh:
	sp <= hl <= bc
	a = *(hl = R + sp) | a jp .1
	a =! a + 1
	jmp .3
.1:
	jz c.lret
	a -> c
	hl =a^ (hl = L + sp)
.2:
	hl + hl
	c - 1 jnz .2
	jmp .7

c.irsh:
	sp <= hl <= bc
	a = *(hl = R + sp) | a jp .3
	a =! a + 1
	jmp .1
.3:
	jz c.lret
	a -> c
	hl =a^ (hl = L + sp)
.4:
	a = h + a = h <^> -1 -> h = l <^> -1 -> l
	c - 1 jnz .4
	jmp .7

c.ursh:
	sp <= hl <= bc
	a = *(hl = R + sp) | a jp .5
	a =! a + 1
	jmp .1
.5:
	jz c.lret
	a -> c
	hl =a^ (hl = L + sp)
.6:
	a = h <-> -1 -> h = l <^> -1 -> l
	c - 1 jnz .6
.7:
	bc = hl ->^ (hl = L + sp)
	jmp c.lret
