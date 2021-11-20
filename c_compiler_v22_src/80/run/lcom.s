/	lcom
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	long complement
/	stack: hl, pc, L

	public	c.lcom
L := 4
c.lcom:
	hl => sp =a^ (hl = L + sp)
	a = *hl =! a -> *hl
	a = *(hl + 1) =! a -> *hl
	a = *(hl + 1) =! a -> *hl
	a = *(hl + 1) =! a -> *hl
	hl <= sp
	ret
