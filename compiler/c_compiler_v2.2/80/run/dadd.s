/	dadd
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	add double to double
/	stack: autos, de, bc, hl, pc, R, L

	public	c.dadd
C := -20
E := -18
O := -9
R := 8
L := 10
c.dadd:
	sp <= hl <= bc <= de
	hl = 0 + sp <> de = -20 + sp -> sp
	bc =^ (hl = R + de) => sp
	hl = O + de => sp
	call c.unpk					/ R into O
	sp => af => af
	a = c | b jz .8				/ x + 0 == x
	bc ->^ (hl = C + de)
	bc =^ (hl = L + de) => sp
	hl = E + de => sp
	call c.unpk					/ L into E
	sp => af => af
	a = c | b jnz .2
	bc =^ (hl = R + de)
	jmp .7						/ 0 + x == x
.2:
	hl = C + de					/ L != 0 && R != 0
	a = c - *hl -> c = b -^ *(hl + 1) -> b
	jm .3
	a = c | b jz .6
	a = *(hl - 1) + c -> *hl = *(hl + 1) +^ b -> *hl	/ R < L
	hl = O[8] + de
	jmp .4
.3:
	a - a - c -> c = 0 -^ b -> b	/ L < R
	hl = E[8] + de
.4:
	de => sp					/ shift *hl right by bc
.41:
	d = 9
	a = *hl + a; hl + 1
.42:
	a = *(hl - 1) <^> -1 -> *hl
	d - 1 jnz .42
	jnc .5
	a | 1 -> *hl				/ sticky bit
.5:
	bc - 1
	a = c | b jz .51
	hl + (de = 8)
	jmp .41
.51:
	de <= sp
.6:
	bc = (hl = O[-1] + de)
	hl = E[-1] + de					/ add O into E
	de => sp; d = 9; a|a
.61:
	a = *(bc + 1) +^ *(hl + 1) -> *hl
	d - 1 jnz .61
	de <= sp
	bc =^ (hl = C + de) => sp
	hl = E + de => sp
	call c.repk
	sp => af => af
	bc = (hl = E + de)
.7:
	bc - 1						/ bc is src to copy
	hl =a^ (hl = L + de) - 1
	de => sp; d = 8
.71:
	a = *(bc + 1) -> *(hl + 1)
	d - 1 jnz .71
	de <= sp
.8:
	sp = (hl <> de) => de
	jmp c.lret
