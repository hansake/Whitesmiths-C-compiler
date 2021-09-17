/	switch
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	C runtime for switch statement
/	bc = switch value
/	hl: (lbl, val), ... (0, default)
/
/	for (; hl->lbl; hl =+ 4)
/		if (hl->val == bc)
/			goto *hl->lbl
/	goto *hl->default

	public	c.switch
c.switch:
	a = *hl | *(hl + 1) jnz .0
	hl =a^ (hl + 1)		/default
	jmp *hl
.0:
	a = c :: *(hl + 1) jnz .1 = b :: *(hl + 1) jnz .2
	hl =a^ (hl - 1 - 1 - 1)
	jmp *hl
.1:
	hl + 1
.2:
	hl + 1
	jmp c.switch
