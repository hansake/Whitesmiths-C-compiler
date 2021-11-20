/	lneg
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	long negate
/	stack: hl, pc, L

	public	c.lneg
L := 4
c.lneg:
	hl => sp =a^ (hl = L + sp)
	a - a - *(hl + 1 + 1) -> *hl
	a = 0 -^ *(hl + 1) -> *hl
	a = 0 -^ *(hl - 1 - 1 - 1) -> *hl
	a = 0 -^ *(hl + 1) -> *hl
	hl <= sp
	ret
