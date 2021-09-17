/	lmul
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	mul long into long
/	stack: Plo, Phi, WRlo, WRhi, WLlo, WLhi, bc, hl, pc, R, L
/
/	P = 0
/	while (WR)
/		if (WR & 1)
/			P =+ WL
/		WL =<< 1
/		WR =>> 1

	public	c.lmul
P := 0
WR := 4
WL := 8
R := 18
L := 20
c.lmul:
	sp <= hl <= bc
	hl =a^ (hl = L[-12] + sp)
	bc =^ hl => sp =^ (hl + 1) => sp
	hl =a^ (hl = R[-8] + sp)
	bc =^ hl => sp =^ (hl + 1) => sp
	hl = 0 => sp => sp
.1:
	a = *(hl = WR[3] + sp) | *(hl - 1) | *(hl - 1) | *(hl - 1) jz .3
	a = *hl <^> -1 jnc .2
	hl = P + sp -> bc = WL + sp
	a = *bc + *hl -> *bc
	a = *(bc + 1) +^ *(hl + 1) -> *bc
	a = *(bc + 1) +^ *(hl + 1) -> *bc
	a = *(bc + 1) +^ *(hl + 1) -> *bc
.2:
	hl = WL + sp
	a = *hl + a -> *hl
	a = *(hl + 1) +^ a -> *hl
	a = *(hl + 1) +^ a -> *hl
	a = *(hl + 1) +^ a -> *hl
	hl = WR[3] + sp
	a = *hl <-> -1 -> *hl
	a = *(hl - 1) <^> -1 -> *hl
	a = *(hl - 1) <^> -1 -> *hl
	a = *(hl - 1) <^> -1 -> *hl
	jmp .1
.3:
	bc =^ (hl = L + sp)
	hl = P[2] + sp
	a = *hl -> *bc = *(hl + 1) -> *(bc + 1)
	hl <= sp
	a = l -> *(bc + 1) = h -> *(bc + 1)
	hl = 10 + sp -> sp
	jmp c.lret
