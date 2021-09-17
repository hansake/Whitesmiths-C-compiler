/	dsub
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	subtract double from double
/	stack: autos, de, bc, hl, pc, R, L

	public c.dsub
NR := -8
R := 8
L := 10
c.dsub:
	sp <= hl <= bc <= de
	hl = 0 + sp <> de = -8 + sp -> sp
	bc =^ (hl = R + de) - 1
	hl = NR[-1] + de
	de => sp; e = 8
.1:
	a = *(bc + 1) -> *(hl + 1)
	e - 1 jnz .1
	de <= sp
	a = *(hl = NR[1] + de) + 0200 -> *hl
	bc =^ (hl = L + de) => sp
	hl = NR + de => sp
	call c.dadd
	sp = (hl <> de) => de
	jmp c.lret
