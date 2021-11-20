# Whitesmiths, Ltd. C Compiler

## Background

The original Whitesmiths compiler was released in 1978 and compiled a version of C similar
to that accepted by Version 6 Unix. It was an entirely new implementation, borrowing no code from Unix.

When Whitesmiths released version 3.0 of its C compiler in 1985 it supported the emerging ANSI C standard.

The Whitesmiths compiler had code generators for DEC PDP-11, Intel 8080/Zilog Z80, Intel 8086, Motorola MC68000,
DEC VAX-11, IBM System/370 and IBM System/36. It was commonly used as a cross compiler.

The company's president from 1978 to 1988 was P. J. Plauger.

[Whitesmiths - Wikipedia](https://en.wikipedia.org/wiki/Whitesmiths)

## Making the C compilers available for download

As the Whitesmiths C compilers are a very important contribution to
the development of the use of the C language, I think that it is important
that this software is not lost for the future.

I recieved an e-mail reply from P.J. Plauger on 3 Apr 2021 where he is positive to
making the compiler available for download.

\<pjp>
  
I own all rights to the Whitesmiths C compiler. if you want to go to the trouble
to package it so that it can be downloaded from such a site, II'll be happy to
supply you with a letter giving limited rights to use it at no cost, at least for
non-commerecial purposes, provided you supply me with a copy of the
package you make.

Glad you're still active after all these decades. I am too, but definitely slowed
down by age.

Best regards,

P.J. Plauger
  
\</pjp>

I have tried to get in contact with him to confirm that it is ok to put the
C compiler in a public repository, but unfortunately I have not got any answer yet.

## Where to find the different compilers

The Whitesmiths C Compiler for CP/M-80 Edition 2.2 in binary format is in the "ccpm80_v22_bin" directory.
The manuals for Edition 2.2 compilers are in the "compiler_v22_doc" directory.

The Whitesmiths C Cross Compiler an a MS-DOS host for IBM Systerm/36 Version 3.1 in binary format is in the "cx36_v31_bin" directory. A QEMU disk image with MS-DOS where thw compiler is installed is also in this directory.
The manuals for this compiler are in the "compiler_s36_v31_doc" directory.

The source code for the CP/M-80 Compiler Edition 2.2 and Makefiles to build it are available in the "c_compiler_v22_src" directory. The source should be for edition 2.2 but there might be some source coming from other releases. The intention is to add more hosts and targets to this build environment.

Manuals for the Whitesmiths/COSMIC C Cross Compiler Version 3.32 for target Z80 is in the "compiler_z80_v332_doc" directory. Eventually also the compiler will be added to this repository.
