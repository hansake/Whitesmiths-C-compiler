/	ldiv
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	div long into long
/	stack: Qlo, Qhi, Nlo, Nhi, WRlo, WRhi, WLlo, WLhi, de, bc, hl, pc, R, L

	public	c.ldiv
	public	c.lmod
	public	c.uldiv
	public	c.ulmod
Q := 0
N := 4
WR := 8
WL := 12
R := 24
L := 26
/	internal divide routine
/	S = 0
/	if (R < 0)
/		R = -R, ++S
/	if (L < 0)
/		L = -L, ++S
/	(N,Q) = (0,L)
/	for (32 bits)
/		(N,Q) =<< 1
/		if (R <= N)
/			N =- R, ++Q
divide:
	bc = (hl = Q[2] + sp)
	hl = WL[2] + sp
	a = *hl -> *bc
	a = *(hl + 1) -> *(bc + 1)
	a = *(hl + 1) -> *(bc + 1)
	a = *(hl + 1) -> *(bc + 1)
	a - a -> *(bc + 1) -> *(bc + 1) -> *(bc + 1) -> *(bc + 1)
	d = 32
.0:
	d - 1; rm
	hl = Q[2] + sp
	a = *hl + a -> *hl
	a = *(hl + 1) +^ a -> *hl
	a = *(hl + 1) +^ a -> *hl
	a = *(hl + 1) +^ a -> *hl
	a = *(hl + 1) +^ a -> *hl
	a = *(hl + 1) +^ a -> *hl
	a = *(hl + 1) +^ a -> *hl
	a = *(hl + 1) +^ a -> *hl
	bc = (hl = N[2] + sp)
	hl = WR[2] + sp
	a = *bc - *hl
	a = *(bc + 1) -^ *(hl + 1)
	a = *(bc + 1) -^ *(hl + 1)
	a = *(bc + 1) -^ *(hl + 1)
	jc .0
	bc = (hl = N[2] + sp)
	hl = WR[2] + sp
	a = *bc - *hl -> *bc
	a = *(bc + 1) -^ *(hl + 1) -> *bc
	a = *(bc + 1) -^ *(hl + 1) -> *bc
	a = *(bc + 1) -^ *(hl + 1) -> *bc
	*(hl = Q[2] + sp) + 1
	jmp .0

/	enter and save registers
enter:
	hl <> *sp
	sp <= bc <= de
	hl <> de = -16 + sp -> sp <> de
	e = 0
	jmp *hl

/	exit and restore registers
exit:
	hl = 16 + sp -> sp
	de <= sp
	jmp c.lret

/	bc = dst(lsb), hl = src(msb)
get:
	a = *(hl + 1) -> *bc
	a = *(hl + 1) -> *(bc + 1)
	a = *(hl - 1 - 1 - 1) -> *(bc + 1)
	a = *(hl + 1) -> *(bc + 1)
	ret

/	bc = dst, e = S flag, hl = src
getabs:
	a = *(hl + 1) | a jp get
	a - a - *(hl + 1) -> *bc
	a = 0 -^ *(hl + 1) -> *(bc + 1)
	a = 0 -^ *(hl - 1 - 1 - 1) -> *(bc + 1)
	a = 0 -^ *(hl + 1) -> *(bc + 1)
	e + 1
	ret

/	bc = dst, hl = src
put:
	a = *hl -> *(bc + 1 + 1)
	a = *(hl + 1) -> *(bc + 1)
	a = *(hl + 1) -> *(bc - 1 - 1 - 1)
	a = *(hl + 1) -> *(bc + 1)
	ret

/	bc = dst, e = S flag, hl = src
putabs:
	a = e & 1 jz put
	a - a - *hl -> *(bc + 1 + 1)
	a = 0 -^ *(hl + 1) -> *(bc + 1)
	a = 0 -^ *(hl + 1) -> *(bc - 1 - 1 - 1)
	a = 0 -^ *(hl + 1) -> *(bc + 1)
	ret

c.ldiv:
	call enter
	bc = (hl = WR + sp)
	hl =a^ (hl = R + sp)
	call getabs
	bc = (hl = WL + sp)
	hl =a^ (hl = L + sp)
	call getabs
	call divide
	bc =^ (hl = L + sp)
	hl = Q + sp
	call putabs
	jmp exit

c.lmod:
	call enter
	bc = (hl = WR + sp)
	hl =a^ (hl = R + sp)
	call getabs
	e = 0
	bc = (hl = WL + sp)
	hl =a^ (hl = L + sp)
	call getabs
	call divide
	bc =^ (hl = L + sp)
	hl = N + sp
	call putabs
	jmp exit

c.uldiv:
	call enter
	e = Q
udiv:
	bc = (hl = WL + sp)
	hl =a^ (hl = L + sp) + 1
	call get
	bc = (hl = WR + sp)
	hl =a^ (hl = R + sp) + 1
	call get
	call divide
	bc =^ (hl = L + sp)
	d = 0; hl = de + sp
	call put
	jmp exit

c.ulmod:
	call enter
	e = N
	jmp udiv
