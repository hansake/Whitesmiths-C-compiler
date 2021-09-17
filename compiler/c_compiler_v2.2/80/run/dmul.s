/	dmul
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	multiply double into double
/	stack: autos, de, bc, hl, pc, R, L

	public c.dmul
P := -31
N := -22
S := -21
C := -20
E := -18
O := -9
R := 8
L := 10
c.dmul:
	sp <= hl <= bc <= de
	hl = 0 + sp <> de = -31 + sp -> sp
	bc =^ (hl = L + de)
	*(hl = S + de) = 0
	a = *(bc + 1) | a jp .0
	a + 0200 -> *bc					/ L < 0
	*hl + 1
.0:
	bc - 1 => sp
	hl = E + de => sp
	call c.unpk						/ L into E
	sp => af => af
	a = c | b jz .6					/ 0 * x == 0
	bc ->^ (hl = C + de)
	bc =^ (hl = R + de) => sp
	hl = O + de => sp
	call c.unpk						/ R into O
	sp => af => af
	a = c | b jnz .1
	bc =^ (hl = L + de) - 1			/ x * 0 = 0
	a - a; h = 8
.01:
	a -> *(bc + 1)
	h - 1 jnz .01
	jmp .6
.1:
	a = *(hl = C + de) + c -> c = *(hl + 1) +^ b -> b
	a = c - 0200 -> *(hl - 1) = b -^ 0 -> *(hl + 1)
	hl = P[-1] + de					/ P = 0
	a - a; c = 9
.11:
	a -> *(hl + 1)
	c - 1 jnz .11
	a = 56 -> *(hl = N + de)
.2:
	hl = O[8] + de					/ O =>> 1
	c = 9; a = *hl + a; hl + 1
.21:
	a = *(hl - 1) <^> -1 -> *hl
	c - 1 jnz .21
	a = *(hl = E[7] + de) | a jp .3	/ if (msb E > O)
	bc = (hl = P[-1] + de)			/ P =+ O
	hl = O[-1] + de
	de => sp; e = 9; a | a
.22:
	a = *(bc + 1) +^ *(hl + 1) -> *bc
	e - 1 jnz .22
	de <= sp
.3:
	hl = E[-1] + de					/ E =<< 1
	c = 8; a | a
.31:
	a = *(hl + 1) <^> 1 -> *hl
	c - 1 jnz .31
	*(hl = N + de) - 1 jnz .2
	bc =^ (hl = C + de) => sp		/ P now evaluated
	hl = P + de => sp
	call c.repk
	sp => af => af
	a = *(hl = S + de) | a jz .5
	a = *(hl = P + de) | *(hl + 1) jz .5
	a = *hl ^ 0200 -> *hl
.5:
	bc =^ (hl = L + de) - 1
	hl = P[-1] + de
	de => sp; e = 8
.51:
	a = *(hl + 1) -> *(bc + 1)
	e - 1 jnz .51
	de <= sp
.6:
	sp = (hl <> de) => de
	jmp c.lret
