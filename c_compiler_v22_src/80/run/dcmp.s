/	dcmp
/	copyright (c) 1979 by Whitesmiths, Ltd.
/	compare double to double, set NCZ
/	stack: de, bc, hl, pc, R, L

	public	c.dcmp
R := 8
L := 10
c.dcmp:
	sp <= hl <= bc <= de
	bc =^ (hl = L + sp =a^ hl)
	a = b | a jp .1
	a - a - c -> c = 0200 -^ b -> b
.1:
	de =^ (hl = R + sp =a^ hl)
	a = d | a jp .2
	a - a - e -> e = 0200 -^ d -> d
.2:
	a = b + 0200 -> b = d + 0200 -> d
	a = c - e -> l = b -^ d jc .4
	a | l jz .5
.3:								/ L > R
	a = 1
	jmp .7
.4:								/ L < R
	a = -1
	jmp .7
.5:
	bc =^ (hl = L + sp) + 1
	hl =a^ (hl = R + sp) + 1
	a = *bc | a jp .55
	sp <= bc <= hl => bc => hl
.55:
	e = 3
.6:
	a = *(bc + 1 + 1) :: *(hl + 1 + 1) jnz .8
	a = *(bc - 1) :: *(hl - 1) jnz .8
	e - 1 jnz .6
	a - a						/ L == R
.7:								/ a = answer
	de <= sp
	a + a
	jmp c.zret
.8:
	jc .4
	jmp .3
