/	idiv
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	div int into int
/	stack: (pc, Q, N, S), de, bc, hl, pc, R, L

	public	c.idiv
	public	c.imod
	public	c.udiv
	public	c.umod
Q := -6
R := 8
L := 10
/	internal divide routine
/	S = 0
/	if (R < 0)
/		R = -R, ++S
/	if (L < 0)
/		L = -L, ++S
/	(N,Q) = (0,L)
/	for (16 bits)
/		(N,Q) =<< 1
/		if (R <= N)
/			N =- R, ++Q
divide:
	sp => hl <= de <= (de = 0) <= bc <= hl
	d = 16
.0:
	d - 1; rm
	hl = Q[8] + sp
	a = *hl + a -> *hl
	a = *(hl + 1) +^ a -> *hl
	a = *(hl + 1) +^ a -> *hl
	a = *(hl + 1) +^ a -> *hl
	hl - 1 -> bc = R[8] + sp
	a = *bc - *hl -> e = *(bc + 1) -^ *(hl + 1) jc .0
	a -> *bc = e -> *(bc - 1)
	*(hl = Q[8] + sp) + 1
	jmp .0

c.idiv:
	sp <= hl <= bc <= de
	e = 0
	a = *(hl = R[1] + sp) | a jp .1
	a - a - *(hl - 1) -> *hl = 0 -^ *(hl + 1) -> *hl
	e + 1
.1:
	bc =^ (hl = L + sp)
	a = b | a jp .2
	a - a - c -> c = 0 -^ b -> b
	e + 1
.2:
	call divide
	sp => bc => af => af jnc .3	/ lsb(e) to carry
	a - a - c -> c = 0 -^ b -> b
.3:
	bc ->^ (hl = L + sp)
	de <= sp
	jmp c.lret

c.imod:
	sp <= hl <= bc <= de
	e = 0
	a = *(hl = R[1] + sp) | a jp .4
	a - a - *(hl - 1) -> *hl = 0 -^ *(hl + 1) -> *hl
.4:
	bc =^ (hl = L + sp)
	a = b | a jp .5
	a - a - c -> c = 0 -^ b -> b
	e + 1
.5:
	call divide
	sp => af => bc => af jnc .6	/ lsb(e) to carry
	a - a - c -> c = 0 -^ b -> b
.6:
	bc ->^ (hl = L + sp)
	de <= sp
	jmp c.lret

c.udiv:
	sp <= hl <= bc <= de
	bc =^ (hl = L + sp)
	call divide
	sp => bc => af => af
	bc ->^ (hl = L + sp)
	de <= sp
	jmp c.lret

c.umod:
	sp <= hl <= bc <= de
	bc =^ (hl = L + sp)
	call divide
	sp => af => bc => af
	bc ->^ (hl = L + sp)
	de <= sp
	jmp c.lret
