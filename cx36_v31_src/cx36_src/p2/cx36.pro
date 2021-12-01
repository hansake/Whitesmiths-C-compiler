#	IBM S/36 PROTO FILE
#
#	CURRENT PROGRAMMABLE FLAG OPTIONS:
#
#	catg	: set category of object member to 20
#	ck		: turn on check flag
#	cs		: create assembler source with c code as comments
#	debug	: include debugging information (turn on +debug to p1)
#	lincl	: include header files in listing or diagnostic output
#	m		: name module same as output file
#	p		: turn on profiler flag
#	std		: ANSI C standard
#

sp:pp		-o (o) (i)
asm:

c:pp		-o (o) {lincl?+lincl}  \
			+map /cx36/atoe \
			-i "|/cx36/hdrs/" -x /cx36/hdrs/ssp36n.h (i)
1:p1		-o (o) -b0 -cmu {debug?+debug} {std?+std} -r0 (i)
2:p236 -o (o) {m?-m (r)} {dump?+d1} \
			{catg?-catg 20} \
			{cs?+list} {ck?-ck} {p?-p} (i)
	{cs?lm	-lt -c"* " -o (r).cs (o)}
	{cs?copy (r).cs (o)}
	{cs?del (r).cs}
asm:
