/	dtf
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	convert double to float
/	stack: bc, hl, pc, R, L

	public c.dtf
R := 6
L := 8
c.dtf:
	sp <= hl <= bc
	bc =^ (hl = R + sp)
	hl =a^ (hl = L + sp)
	a = *bc -> *hl
	a = *(bc + 1) -> *(hl + 1)
	a = *(bc + 1) -> *(hl + 1)
	a = *(bc + 1) -> *(hl + 1)
	a = *(bc + 1 + 1) | a jp c.lret
	*(hl - 1) + 1 jnz c.lret		/ round up
	*(hl + 1) + 1 jnz c.lret
	*(hl - 1 - 1 - 1) + 1 jnz c.lret
	*(hl + 1) + 1
	a = *hl & 0177 jnz c.lret
	*hl - 1
	a = -1 -> *(hl - 1)	/ overflow, set huge
	a -> *(hl + 1 + 1) -> *(hl + 1)
	jmp c.lret
