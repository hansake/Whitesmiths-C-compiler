#	make p1pr, a utility that prints output from p1
#
%define CC cp11
#
%define OBJECTS p2pgcode.o p2pgexpr.o p2pgtbody.o p2pmain.o p2pops.o p2subs.o
#
p1pr: OBJECTS
	CC -o p1pr OBJECTS
setb -b 10000 p1pr
#
p2pgcode.o: p2pgcode.c
	CC +o p2pgcode.c
#
p2pgexpr.o: p2pgexpr.c
	CC +o p2pgexpr.c
#
p2pgtbody.o: p2pgtbody.c
	CC +o p2pgtbody.c
#
p2pmain.o: p2pmain.c
	CC +o p2pmain.c
#
p2pops.o: p2pops.c
	CC +o p2pops.c
#
p2subs.o: p2subs.c
	CC +o p2subs.c
#
install: p1pr
	mv p1pr /u/hl/bin
#
