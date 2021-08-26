# Whitesmiths, Ltd. C Compiler

The original Whitesmiths compiler was released in 1978 and compiled a version of C similar
to that accepted by Version 6 Unix. It was an entirely new implementation, borrowing no code from Unix.

When Whitesmiths released version 3.0 of its C compiler in 1985 it supported the emerging ANSI C standard.

The Whitesmiths compiler had code generators for DEC PDP-11, Intel 8080/Zilog Z80, Intel 8086, Motorola MC68000,
DEC VAX-11, IBM System/370 and IBM System/36. It was commonly used as a cross compiler.

The C compiler in this repository is version 2.2 for Intel 8080/Zilog Z80 and runs under CP/M.
The compiler was delivered on two diskettes and is installed by linking the object files
for the different compiler phases and utilities with the CP/M specific libraries.
For installation see the description in the file "instal.doc".

I got a mail answer from P.J. Plauger giving limited rights to use the software at no cost provided that it
is not used commercially.

The installation and compilation of a simple program (echo.c) has been tested in a simulated
CP/M environment using Z80pack cpmsim:
[udo-munk/z80pack: A Zilog Z80 and Intel 8080 systems emulation](https://github.com/udo-munk/z80pack).
