/	dneg
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	double negate
/	stack: hl, pc, L

	public	c.dneg
L := 4
c.dneg:
	hl => sp =a^ (hl = L + sp)
	a = *hl | *(hl + 1) jz .1
	a = *hl + 0200 -> *hl
.1:
	hl <= sp
	ret
