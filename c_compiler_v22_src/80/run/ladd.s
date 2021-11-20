/	ladd
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	add long to long
/	stack: bc, hl, pc, R, L

	public	c.ladd
R := 6
L := 8
c.ladd:
	sp <= hl <= bc
	bc =^ (hl = L + sp)
	hl =a^ (hl = R + sp)
	a = *(bc + 1 + 1) + *(hl + 1 + 1) -> *bc
	a = *(bc + 1) +^ *(hl + 1) -> *bc
	a = *(bc - 1 - 1 - 1) +^ *(hl - 1 - 1 - 1) -> *bc
	a = *(bc + 1) +^ *(hl + 1) -> *bc
	jmp c.lret
