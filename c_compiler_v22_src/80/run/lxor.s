/	lxor
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	xor long into long
/	stack: bc, hl, pc, R, L

	public	c.lxor
R := 6
L := 8
c.lxor:
	sp <= hl <= bc
	bc =^ (hl = L + sp)
	hl =a^ (hl = R + sp)
	a = *bc ^ *hl -> *bc
	a = *(bc + 1) ^ *(hl + 1) -> *bc
	a = *(bc + 1) ^ *(hl + 1) -> *bc
	a = *(bc + 1) ^ *(hl + 1) -> *bc
	jmp c.lret
