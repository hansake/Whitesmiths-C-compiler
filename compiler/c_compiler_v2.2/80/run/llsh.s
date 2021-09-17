/	lsh
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	long shifts
/	stack: bc, hl, pc, R, L

	public	c.llsh
	public	c.lrsh
	public	c.ulrsh
R := 6
L := 8
c.llsh:
	sp <= hl <= bc
	a = *(hl = R + sp) | a jp .1
	a =! a + 1
	jmp .3
.1:
	jz c.lret
	a -> c
	hl =a^ (hl = L + sp) + 1
.2:
	a = *(hl + 1) + a -> *hl
	a = *(hl + 1) +^ a -> *hl
	a = *(hl - 1 - 1 - 1) +^ a -> *hl
	a = *(hl + 1) +^ a -> *hl
	c - 1 jnz .2
	jmp c.lret

c.lrsh:
	sp <= hl <= bc
	a = *(hl = R + sp) | a jp .3
	a =! a + 1
	jmp .1
.3:
	jz c.lret
	a -> c
	hl =a^ (hl = L + sp) + 1 + 1
.4:
	a = *(hl - 1) + a = *hl <^> -1 -> *hl
	a = *(hl - 1) <^> -1 -> *hl
	a = *(hl + 1 + 1 + 1) <^> -1 -> *hl
	a = *(hl - 1) <^> -1 -> *hl
	c - 1 jnz .4
	jmp c.lret

c.ulrsh:
	sp <= hl <= bc
	a = *(hl = R + sp) | a jp .5
	a =! a + 1
	jmp .1
.5:
	jz c.lret
	a -> c
	hl =a^ (hl = L + sp) + 1 + 1
.6:
	a = *(hl - 1) <-> -1 -> *hl
	a = *(hl - 1) <^> -1 -> *hl
	a = *(hl + 1 + 1 + 1) <^> -1 -> *hl
	a = *(hl - 1) <^> -1 -> *hl
	c - 1 jnz .6
	jmp c.lret
