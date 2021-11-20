/	ddiv
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	divide double into double
/	stack: autos, de, bc, hl, pc, R, L

	public	c.ddiv
Q := -31
N := -22
S := -21
C := -20
E := -18
O := -9
R := 8
L := 10
c.ddiv:
	sp <= hl <= bc <= de
	hl = 0 + sp <> de = -31 + sp -> sp
	*(hl = S + de) = 0				/ S = 0
	bc =^ (hl = R + de) => sp
	hl = O + de => sp
	call c.unpk						/ R into O
	sp => af => af
	a = c | b jnz .0
	bc =^ (hl = L + de) + 1			/ x div 0 = huge
	a = *bc | 0177 -> *bc = 0377 -> *(bc - 1)
	a -> *(bc + 1 + 1) -> *(bc + 1)
	a -> *(bc + 1) -> *(bc + 1)
	a -> *(bc + 1) -> *(bc + 1)
	jmp .8
.0:
	a = *(hl = O[8] + de) | a jp .1
	hl = O + de						/ R < 0, reverse sign
	de => sp; e = 8; a | a			/ guard is zero
.01:
	a = 0 -^ *(hl + 1) -> *hl
	e - 1 jnz .01
	de <= sp
	*(hl = S + de) + 1
.1:
	bc ->^ (hl = C + de)			/ save C of R
	bc =^ (hl = L + de) + 1
	a = *bc | a jp .2
	a & 0177 -> *bc					/ drop sign of L
	*(hl = S + de) + 1
.2:
	bc - 1 => sp
	hl = E + de => sp
	call c.unpk						/ L into E
	sp => af => af
	a = c | b jz .8					/ 0 div x = 0
	a = c - *(hl = C + de) -> c = b -^ *(hl + 1) -> b
	a = c + 0200 -> *(hl - 1) = b +^ 0 -> *(hl + 1)
	a - a; c = 9; hl = Q[-1] + de
.21:
	a -> *(hl + 1)
	c - 1 jnz .21
	*(hl = N + de) = 57				/ for 57 bits
.3:
	hl = Q + de
	c = 8; a | a			/ Q =<< 1
.31:
	a = *(hl + 1) <^> 1 -> *hl
	c - 1 jnz .31
	bc = (hl = E[9] + de)			/ if (O <= E)
	hl = O[9] + de
	de => sp; e = 9
.32:
	a = *(bc - 1) :: *(hl - 1) jnz .4
	e - 1 jnz .32
.4:
	de <= sp
	jc .5
	bc = (hl = E[-1] + de)
	hl = O[-1] + de
	de => sp; e = 9; a | a			/ E =- O
.41:
	a = *(bc + 1) -^ *(hl + 1) -> *bc
	e - 1 jnz .41
	de <= sp
	*(hl = Q[1] + de) + 1			/ ++Q
.5:
	hl = O[9] + de					/ O =>> 1
	c = 9; a | a
.51:
	a = *(hl - 1) <^> -1 -> *hl
	c - 1 jnz .51
	*(hl = N + de) - 1 jnz .3
	bc =^ (hl = C + de) => sp		/ Q now evaulated
	hl = Q + de => sp
	call c.repk
	sp => af => af
	a = *(hl = S + de) & 1 jz .7
	a = *(hl = Q + de) | *(hl + 1) jz .7
	a = *hl | 0200 -> *hl
.7:
	bc =^ (hl = L + de) - 1
	hl = Q[-1] + de
	de => sp; e = 8
.71:
	a = *(hl + 1) -> *(bc + 1)
	e - 1 jnz .71
	de <= sp
.8:
	sp = (hl <> de) => de
	jmp c.lret
