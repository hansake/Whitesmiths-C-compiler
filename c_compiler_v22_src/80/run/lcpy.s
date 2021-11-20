/	lcpy
/	copyright (c) 1981 by Whitesmiths, Ltd.
/	copy long to long or double to double
/	stack: pc  source: *hl  dest: *bc

	public	c.dcpy
	public	c.lcpy
c.dcpy:
	a = *hl -> *bc
	a = *(hl + 1) -> *(bc + 1)
	a = *(hl + 1) -> *(bc + 1)
	a = *(hl + 1) -> *(bc + 1)
	hl + 1
	bc + 1
c.lcpy:
	a = *hl -> *bc
	a = *(hl + 1) -> *(bc + 1)
	a = *(hl + 1) -> *(bc + 1)
	a = *(hl + 1) -> *(bc + 1)
	ret
