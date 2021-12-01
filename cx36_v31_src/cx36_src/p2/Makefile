#
# make p2.36 version 3.0
#
# p236.exe generates IBM S/36 assembler code
# p236w.exe generates Whitesmiths/RTS cross assembler code
#
CC = c

OBJECTS = p2esc.o p2expr.o p2gen.o \
		p2gets.o p2jump.o p2main.o p2subs.o p2tab.o

all: p236.exe p236w.exe

p236.exe: p2conx.o $(OBJECTS)
	$(CC) -o p236.exe p2conx.o $(OBJECTS)

p236w.exe: p2conw.o $(OBJECTS)
	$(CC) -o p236w.exe p2conw.o $(OBJECTS)

p2conx.o: p2conx.c p2conf.c int2.h
	$(CC) +o p2conx.c

p2conw.o: p2conw.c p2conf.c int2.h
	$(CC) +o p2conw.c

p2dump.o: p2dump.c int2.h
	$(CC) +o p2dump.c

p2esc.o: p2esc.c int2.h
	$(CC) +o p2esc.c

p2expr.o: p2expr.c int2.h
	$(CC) +o p2expr.c

p2gen.o: p2gen.c int2.h
	$(CC) +o p2gen.c

p2gets.o: p2gets.c int2.h
	$(CC) +o p2gets.c

p2jump.o: p2jump.c int2.h
	$(CC) +o p2jump.c

p2main.o: p2main.c int2.h
	$(CC) +o p2main.c

p2subs.o: p2subs.c int2.h
	$(CC) +o p2subs.c

p2tab.o: p2tab.c int2.h
	$(CC) +o p2tab.c

install: p236w.exe cx36w.pro
	copy p236w.exe c:\cx36
	copy cx36w.pro c:\cx36
