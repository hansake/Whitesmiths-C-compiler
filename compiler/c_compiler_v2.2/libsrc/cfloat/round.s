L2:&040000,&0,&0,&0
L4:&040000,&0,&0,&0
public _round
_round:
call c.ent
hl=&04+de
a=*(hl+1)|a
jp L6
hl=&04+de
sp<=hl
hl<=sp
bc=&c.r0;call c.dcpy
hl=&c.r0=>sp
hl=&L2=>sp
call c.dsub
sp=>af
jmp L01
L6:
hl=&04+de
sp<=hl
hl<=sp
bc=&c.r0;call c.dcpy
hl=&c.r0=>sp
hl=&L4=>sp
call c.dadd
sp=>af
L01:
hl=&c.r0=>sp
bc<=sp=>sp=>sp;call c.dtr;sp=>bc
jmp c.ret
