/	lclt
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	compare long to long, set N and C
/	stack: bc, hl, pc, R, L

	public	c.lclt
R := 6
L := 8
c.lclt:
	sp <= hl <= bc
	bc =^ (hl = L + sp)
	hl =a^ (hl = R + sp)
	a = *(bc + 1 + 1) - *(hl + 1 + 1)
	a = *(bc + 1) -^ *(hl + 1)
	a = *(bc - 1 - 1 - 1) -^ *(hl - 1 - 1 - 1)
	a = *(bc + 1) -^ *(hl + 1)
	jmp c.zret
