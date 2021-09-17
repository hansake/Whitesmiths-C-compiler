/	ent
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	enter and leave functions
/	stack: de, pc, a1, ...

	public	c.ent
	public	c.ihl
	public	c.ret
c.ent:
	hl = 0 + sp <> de <> *sp
c.ihl:
	jmp *hl

c.ret:
	sp = (hl <> de) => de
	ret
