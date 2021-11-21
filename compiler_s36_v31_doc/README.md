In 1984 to 1986 I was working at Unisoft AB and among other tasks I developed
a Whitesmiths C compiler code generator for IBM System/36.

The IBM System/36 has a rather strange architecture compared to later computer architectures.
The maximum addressable memory is 64 Kbytes although there is support in the OS for overlays.
There is no hardware stack implemented so this had to be made in software for the compiler.
The machine can handle arithmetic on 1 to 256 byte binary numbers or 1 to 16 or 32 byte zoned decimal numbers,
(zoned decimal is some kind of BCD i.e. binary coded decimal).

System/36 is obviously based on IBM System/3 and this explains the rather archaic design.
* [SC21-7908-3_System_36_Programming_with_Assembler_Jan86.pdf](http://www.bitsavers.org/pdf/ibm/system36/SC21-7908-3_System_36_Programming_with_Assembler_Jan86.pdf)
* [SC21-9019-5_System_36_Concepts_and_Programmers_Guide_198706.pdf](http://www.bitsavers.org/pdf/ibm/system36/SC21-9019-5_System_36_Concepts_and_Programmers_Guide_198706.pdf)

The main programming language for this system was RPG-II.

I have been told that the IBM word processing program Display Write was ported to System/36 but could not be implemented
on the main CPU (MSP: main storage processor) so it was instead implemented on the i/o processor (CSP: control storage processor) which was mostly undocumented but was said to be a more normal 16 bit processor.
