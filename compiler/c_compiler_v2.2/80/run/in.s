/	in(chan)
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	input from chan
/	stack: pc, chan

	public	_in
_in:
	sp => bc => hl
	h = ret
	hl => sp = 0155400 => sp = 0 + sp
	call c.ihl					/ stack: nop; in; chan; ret
	sp => hl <= bc
	a -> c - a -> b
	ret
