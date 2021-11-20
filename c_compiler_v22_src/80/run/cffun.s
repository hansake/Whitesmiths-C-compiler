/	cffun
/	copyright (c) 1981 by Whitesmiths, Ltd.
/	math assist functions

	public	__addexp
	public	__domain
	public	__frac
	public	__poly
	public	__range
	public	__unpack
	public	__huge
	public	__tiny
	public	__domerr
	public	__ranerr

/	DOUBLE _addexp(d, n, mesg)	add to binary exponent
__addexp:
	hl = 2[-1] + sp
	bc = &c.r0[-1]
	de => sp
	e = 8
.1:
	a = *(hl + 1) -> *(bc + 1)
	e - 1 jnz .1
	de <= sp
	hl = c.r0
	a = l | h jz .5
	hl + hl
	a = h + *(hl = 10 + sp) -> c = 0 +^ *(hl + 1) jm .2
	jnz .4
	a = c.r0[0] + a -> b
	a = c.r0[1] + a = c <^> -1 -> c.r0[1]
	a = b <^> -1 -> c.r0[0]
	ret
.2:
	hl = &c.r0[-1]			/ underflow
	c = 8
	a - a
.3:
	a -> *(hl + 1)
	c - 1 jnz .3
	ret
.4:
	bc =^ (hl = 12 + sp) => sp
	call __range			/ overflow
	sp => af
.5:
	ret

/	VOID _domain(mesg)	report domain error
__domain:
	hl = 2 + sp =a^ hl -> __domerr
	hl = &__domerr => sp = 0 => sp
	call __raise

/	COUNT _frac(pd, mul)	extract integer from fraction
F := -17
X := -8
P := 4
M := 6
__frac:
	call c.ent
	hl = F + sp -> sp
	bc =^ (hl = P + de) => sp
	hl = M + de => sp
	call c.dmul
	sp => af
	a = *(hl = bc + 1) | *(hl - 1) jnz .21
.20:
	bc = 0
	jmp c.ret
.21:
	bc => sp
	hl = F + de => sp
	call c.unpk
	sp => af => af
	a = *(hl = F[8] + de) -> *(hl + 1)	/ extend sign into X[0]
	a = c - 0200 jm .20
	jz .23
	a -> c
.22:
	b = 9					/ shift F[1] ... F[8], X[0]
	hl = F[0] + de
	a | a
.225:
	a = *(hl + 1) <^> 1 -> *hl
	b - 1 jnz .225
	c - 1 jnz .22
.23:
	a = *(hl = X[0] + de) -> *(hl + 1)	/ copy down int part to X
	a = *(hl - 1 - 1) -> *(hl + 1)
	a - a -> *(hl - 1) = *(hl - 1) | a jp .24
	*(hl + 1) - 1			/ if (0.5 < frac) frac =- 1.0
	a = *(hl + 1) + 0401 -> *hl = *(hl + 1) +^ 0 -> *hl
.24:
	hl = 0200 => sp = F + de => sp
	call c.repk
	sp => af => af
	bc =^ (hl = P + de) - 1
	hl = F[-1] + de
	de => sp
	e = 8
.25:
	a = *(hl + 1) -> *(bc + 1)
	e - 1 jnz .25
	de <= sp
	bc =^ (hl = X + de)		/ X(de) is int part
	jmp c.ret

/	DOUBLE _poly(d, table, n)	compute polynomial
__poly:
	de => sp
	bc =^ (hl = 10[2] + sp) - 1
	hl = &c.r0[-1]
	e = 8
.29:
	a = *(bc + 1) -> *(hl + 1)
	e - 1 jnz .29
	bc + 1
	de =^ (hl = 12[2] + sp)
	hl = &c.r0 => sp
.30:
	a = d | e jz .31
	hl = 2[4] + sp => sp
	call c.dmul
	bc => sp
	call c.dadd
	bc = (hl = 8 + bc)
	de - 1
	jmp .30
.31:
	sp => af => de
	ret

/	DOUBLE _range(mesg)	report range error
__range:
	hl = __ranerr
	a = l | h jz .41
	c = 8
	hl = &c.r0[-1]
	a = 0377
.40:
	a -> *(hl + 1)
	c - 1 jnz .40
	a = 0177 -> c.r0[1]
	ret
.41:
	hl =a^ (hl = 2 + sp) -> __ranerr
	hl = &__ranerr => sp = 0 => sp
	call __raise

/	COUNT _unpack(pd)	unpack binary exponent
__unpack:
	hl =a^ (hl = 2 + sp)
	bc =^ hl
	a = b | c; rz
	a = c <^> 1 = b <^> 1 - 0200 -> c + a -^ a -> b
	a = *hl & 0200 | 0100 -> *hl
	a = *(hl - 1) & 0177 -> *hl
	ret

.:=.data
__huge:
	&077777, &-1, &-1, &-1
__tiny:
	&0200, &0, &0, &0
__domerr:
	&0
__ranerr:
	&0
