/	repk
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	repack double in place
/	stack: de, pc, P, C
/	bytes: 0 1 2 3 4 5 6 7 8 => 7 8 5 6 3 4 1 2 x

	public c.repk
P := 4
C := 6
c.repk:
	sp <= de
	d = 0							/ S = 0
	bc =^ (hl = P + sp)
	hl = 8 + bc
	a = *hl | a jp .1
	hl = bc - 1						/ F < 0, negate
	e = 9; a | a
.0:
	a = 0 -^ *(hl + 1) -> *hl
	e - 1 jnz .0
	d - 1							/ --S
	jmp .2
.1:
	e = 8
.11:
	a | *(hl - 1) jnz .2
	e - 1 jnz .11
	jmp .8
.2:
	a = *(hl = 8 + bc) | a jz .3	/ while (frac >= 1.0)
	hl + 1; e = 9; a | a			/ frac =>> 1
.21:
	a = *(hl - 1) <^> -1 -> *hl
	e - 1 jnz .21
	a = *(hl = C + sp) + 0401 -> *hl = *(hl + 1) +^ 0 -> *hl
	jmp .2
.3:
	a = *(hl = 7 + bc) | a jm .4	/ while (frac < 0.5)
	hl = bc - 1; e = 9; a | a		/ frac =<< 1
.31:
	a = *(hl + 1) +^ a -> *hl
	e - 1 jnz .31
	a = *(hl = C + sp) - 0401 -> *hl = *(hl + 1) -^ 0 -> *hl
	jmp .3
.4:
	a = *bc | a jp .5 - a -> *bc
	hl = bc; e = 8; stc				/ round
.41:
	a = *(hl + 1) +^ 0 -> *hl
	e - 1 jnz .41
	jmp .2
.5:
	a = *(hl = C[1] + sp) | a jp .6	/ normalized
.51:
	hl = bc - 1; e = 8; a - a		/ underflow
.52:
	a -> *(hl + 1)
	e - 1 jnz .52
	jmp .8
.6:
	jz .7
	hl = bc; e = 8; a = -1			/ overflow
.61:
	a -> *(hl + 1)
	e - 1 jnz .61
	a = 0177 | d -> *hl				/ signed huge
.7:
	hl = 8 + bc => sp = C[2] + sp
	a = *hl | a jz .51
	a = d + a = *hl <^> -1 -> *(hl <= sp)
	a = *(hl - 1) <^> 1 <*> -1 -> *bc
	a = *(bc + 1 + 1) -> *hl
	e = 3
.71:
	a = *(hl - 1 - 1) -> *bc	/MAGIC
	a = *(bc + 1 + 1) -> *hl
	e - 1 jnz .71
.8:
	sp => de
	ret
