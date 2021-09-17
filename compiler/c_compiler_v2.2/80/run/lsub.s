/	lsub
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	sub long from long
/	stack: bc, hl, pc, R, L

	public	c.lsub
R := 6
L := 8
c.lsub:
	sp <= hl <= bc
	bc =^ (hl = L + sp)
	hl =a^ (hl = R + sp)
	a = *(bc + 1 + 1) - *(hl + 1 + 1) -> *bc
	a = *(bc + 1) -^ *(hl + 1) -> *bc
	a = *(bc - 1 - 1 - 1) -^ *(hl - 1 - 1 - 1) -> *bc
	a = *(bc + 1) -^ *(hl + 1) -> *bc
	jmp c.lret
