/	land
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	and long into long
/	stack: bc, hl, pc, R, L

	public	c.land
R := 6
L := 8
c.land:
	sp <= hl <= bc
	bc =^ (hl = L + sp)
	hl =a^ (hl = R + sp)
	a = *bc & *hl -> *bc
	a = *(bc + 1) & *(hl + 1) -> *bc
	a = *(bc + 1) & *(hl + 1) -> *bc
	a = *(bc + 1) & *(hl + 1) -> *bc
	jmp c.lret
