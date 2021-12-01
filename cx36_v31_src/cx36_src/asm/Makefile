#
#	Makefile for Whitesmiths/RTS x36 cross assembler
#

CC = c -dnostrict

all: x36.exe

.c.o:
	$(CC) +o $<

X36OBJ = xa0.o xa1.o xa2.o xa3.o xa4.o xa5.o xa6.o \
	xaconf.o xanofp.o xanosd.o xps36.o xts36.o 

x36.exe:	$(X36OBJ)
	$(CC) -o x36.exe $(X36OBJ)

install: x36.exe
	copy x36.exe c:\cx36
