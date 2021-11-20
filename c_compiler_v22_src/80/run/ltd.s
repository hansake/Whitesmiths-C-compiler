/	ltd
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	convert long to double
/	stack: autos, de, bc, hl, pc, R, L

	public	c.itd
	public	c.utd
	public	c.ltd
	public	c.ultd
F := -9
R := 8
L := 10
enter:
	hl <> *sp
	sp <= bc <= de
	hl -> bc = 0 + sp <> de = -9 + sp -> sp
	bc => sp =^ (hl = R + de)
	hl = F + de
	a = 0 -> *hl -> *(hl + 1) -> *(hl + 1)
	hl + 1
	ret

exit:
	a -> *(hl + 1) -> *(hl + 1)
	hl = 0250 => sp
	hl = F + de => sp
	call c.repk
	sp => af => af
	bc =^ (hl = L + de) - 1
	hl = F[-1] + de
	de => sp; e = 8
.1:
	a = *(hl + 1) -> *(bc + 1)
	e - 1 jnz .1
	de <= sp
	sp = (hl <> de) => de
	jmp c.lret

c.itd:
	call enter
	bc ->^ hl
	a = b + a -^ a -> *(hl + 1) -> *(hl + 1)
	jmp exit

c.utd:
	call enter
	bc ->^ hl
	a - a -> *(hl + 1) -> *(hl + 1)
	jmp exit

c.ltd:
	call enter
	a = *(bc + 1 + 1) -> *hl = *(bc + 1) -> *(hl + 1)
	a = *(bc - 1 - 1 - 1) -> *(hl + 1) = *(bc + 1) -> *(hl + 1)
	a + a -^ a
	jmp exit

c.ultd:
	call enter
	a = *(bc + 1 + 1) -> *hl = *(bc + 1) -> *(hl + 1)
	a = *(bc - 1 - 1 - 1) -> *(hl + 1) = *(bc + 1) -> *(hl + 1)
	a - a
	jmp exit
