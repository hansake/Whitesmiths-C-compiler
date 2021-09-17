/	out(chan, val)
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	output val to chan
/	stack: pc, chan, val

	public	_out
_out:
	sp => bc => hl
	h = ret
	hl <> *sp
	a = l
	hl = 0151400 => sp = 0 + sp
	call c.ihl					/ stack: nop; out; chan; ret
	bc => sp
	ret
