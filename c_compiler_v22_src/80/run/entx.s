/	entx
/	copyright (c) 1983 by Whitesmiths, Ltd.
/	enter and leave, saving registers and checking the stack for overflow
/	stack: de, pc, a1, ...

	public	c.entx
.1:
	hl = &__memerr => sp = 0 => sp
	call __raise
c.entx:
	hl + sp -> bc = 0 + sp
	a = l - c = h -^ b jc .1
	hl = __stop
	a = c - l = b -^ h jc .1
	bc <= sp
	hl = -2 + sp <> de => sp
	hl = c.r4 => sp = c.r3 => sp = c.r2 => sp = bc
	jmp *hl
