/	ftd
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	move float to double
/	stack: bc, hl, pc, R, L

	public	c.ftd
R := 6
L := 8
c.ftd:
	sp <= hl <= bc
	bc =^ (hl = L + sp)
	hl =a^ (hl = R + sp)
	a = *hl -> *bc = *(hl + 1) -> *(bc + 1)
	a = *(hl + 1) -> *(bc + 1) = *(hl + 1) -> *(bc + 1)
	a - a -> *(bc + 1) -> *(bc + 1) -> *(bc + 1) -> *(bc + 1)
	jmp c.lret
