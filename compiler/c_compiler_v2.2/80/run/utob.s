/	utob
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	convert unsigned to bits
/	stack: de, bc, hl, pc, offset/size, unsigned, ptr to bits

	public	c.utob
O := 8
S := 9
U := 10
P := 12
c.utob:
	sp <= hl <= bc <= de
	a = *(hl = S + sp)
	hl = 1
.2:
	a - 1 jm .3
	hl + hl
	jmp .2
.3:
	hl - 1 <> de
	a = *(hl = O + sp) -> b
	hl <> de
.4:
	a - 1 jm .5
	hl + hl
	jmp .4
.5:
	hl <> de
	hl =a^ (hl = U + sp)
.6:
	b - 1 jm .7
	hl + hl
	jmp .6
.7:
	a = l & e -> c = h & d -> b
	hl =a^ (hl = P + sp)
	a = e =! a & *hl | c -> *hl
	a = d =! a & *(hl + 1) | b -> *hl
	de <= sp
	jmp c.zret
