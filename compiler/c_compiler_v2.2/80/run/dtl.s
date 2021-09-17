/	dtl
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	convert double to long
/	stack: autos, de, bc, hl, pc, R, L

	public c.dti
	public c.dtl
	public c.dtr
F := -9
R := 8
L := 10
unpack:
	hl <> *sp
	sp <= bc <= de
	hl -> bc = 0 + sp <> de = -9 + sp -> sp
	bc => sp =^ (hl = R + de) => sp
	hl = F + de => sp
	call c.unpk
	sp => af => af
	a = c | b jz .6
	a = c - 0250 -> c = b -^ 0 -> b jp .3
.2:
	hl = F[8] + de				/	shift right
	de => sp
	e = 6; a = *hl + a; hl + 1
.21:
	a = *(hl - 1) <^> -1 -> *hl
	e - 1 jnz .21
	de <= sp
	bc + 1; a = c | b jnz .2
	jmp .6
.3:
	a = c | b jz .6
	hl = F[2] + de 				/	shift left
	de => sp
	e = 4; a | a
.31:
	a = *(hl + 1) +^ a -> *hl
	e - 1 jnz .31
	de <= sp
	bc - 1
	jmp .3
.6:
	hl = L + de
	ret

c.dti:
	call unpack
	bc =^ hl
.7:
	hl = F[3] + de
	a = *hl -> *bc
	a = *(hl + 1) -> *(bc + 1)
	sp = (hl <> de) => de
	jmp c.lret

c.dtl:
	call unpack
	bc =^ hl
	hl = F[6] + de
	a = *hl -> *(bc + 1) = *(hl - 1) -> *(bc - 1)
	a = *(hl - 1) -> *(bc + 1 + 1 + 1) = *(hl - 1) -> *(bc - 1)
	sp = (hl <> de) => de
	jmp c.lret

c.dtr:
	call unpack
	bc = hl
	jmp .7
