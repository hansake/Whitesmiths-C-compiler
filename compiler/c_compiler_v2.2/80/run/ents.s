/	ents
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	enter and leave, saving registers
/	stack: de, pc, a1, ...

	public	c.ents
	public	c.rets
	public	c.r0
	public	c.r1
	public	c.r2
	public	c.r3
	public	c.r4
c.ents:
	bc <= sp
	hl = -2 + sp <> de => sp
	hl = c.r4 => sp = c.r3 => sp = c.r2 => sp = bc
	jmp *hl

c.rets:
	hl = -6 + de -> sp
	hl <= sp -> c.r2 <= sp -> c.r3 <= sp -> c.r4
	de <= sp
	ret

.:=.data
c.r0:
	0; 0; 0; 0; 0; 0; 0; 0
c.r1:
	0; 0; 0; 0; 0; 0; 0; 0
c.r2:
	0; 0
c.r3:
	0; 0
c.r4:
	0; 0
