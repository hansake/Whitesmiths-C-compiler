/	dtd
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	move double to double
/	stack: bc, hl, pc, R, L

	public	c.dtd
R := 6
L := 8
c.dtd:
	sp <= hl <= bc
	bc =^ (hl = L + sp)
	hl =a^ (hl = R + sp)
	de => sp; e = 7
	a = *hl -> *bc
.1:
	a = *(hl + 1) -> *(bc + 1)
	e - 1 jnz .1
	de <= sp
	jmp c.lret
