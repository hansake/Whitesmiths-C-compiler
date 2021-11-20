/	lcmp
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	compare long to long, set Z
/	stack: bc, hl, pc, R, L

	public	c.lcmp
R := 6
L := 8
c.lcmp:
	sp <= hl <= bc
	bc =^ (hl = L + sp)
	hl =a^ (hl = R + sp)
	a = *bc :: *hl jnz c.zret
	a = *(bc + 1) :: *(hl + 1) jnz c.zret
	a = *(bc + 1) :: *(hl + 1) jnz c.zret
	a = *(bc + 1) :: *(hl + 1)
	jmp c.zret
