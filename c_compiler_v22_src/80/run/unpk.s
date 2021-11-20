/	unpk
/	copyright (c) 1979 Whitesmiths, Ltd.
/	unpack double into P, C
/	stack: S, de, pc, P, D

	public c.unpk
S := 0
P := 6
D := 8
c.unpk:
	sp <= de <= (hl = 0)
	bc =^ (hl = P + sp)
	de =^ (hl = D + sp) - 1
	de <> (hl = 10 + bc)
	bc => sp; c = 4
.0:
	a = *(hl + 1) -> *(de - 1 - 1 - 1)
	a = *(hl + 1) -> *(de + 1)
	c - 1 jnz .0
	bc <= sp
	a - a -> *bc					/clear guard byte
	a = *(hl = 7 + bc) <^> 1
	a = *(hl + 1) <^> 1 -> d jnc .1
	hl <> *sp - 1 <> *sp			/save sign
.1:
	a | a jnz .2
	e = 8
.11:
	a -> *(bc + 1)
	e - 1 jnz .11
	jmp .3
.2:
	a = 0 -> *hl = *(hl - 1) | 0200 -> *hl	/hidden bit
	hl <> *sp
	a = l | a jz .3
	hl = bc; e = 8; a | a
.21:
	a = 0 -^ *(hl + 1) -> *hl		/negate fraction
	e - 1 jnz .21
.3:
	c = d; b = 0
	sp => af => de
	ret
